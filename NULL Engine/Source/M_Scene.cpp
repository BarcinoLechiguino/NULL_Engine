#include "Profiler.h"

#include "Application.h"														// ATTENTION: Globals.h already included in Module.h
#include "M_Window.h"
#include "M_Camera3D.h"
#include "M_Renderer3D.h"
#include "M_Input.h"
#include "M_Editor.h"
#include "Primitive.h"

#include "I_Meshes.h"
#include "I_Materials.h"
#include "R_Mesh.h"
#include "R_Material.h"

#include "GameObject.h"
#include "Component.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"

#include "M_Scene.h"

#include "MemoryManager.h"

M_Scene::M_Scene(bool is_active) : Module("SceneManager", is_active),
root_object(nullptr),
selected_game_object(nullptr)
{

}

M_Scene::~M_Scene()
{

}

bool M_Scene::Init(Configuration& config)
{
	return true;
}

// Load assets
bool M_Scene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->LookAt(vec3(0, 0, 0));

	if (root_object == nullptr)
	{
		root_object = CreateGameObject("Main Scene");

		selected_game_object = root_object;
	}

	CreateGameObjectsFromModel("Assets/Models/baker_house/BakerHouse.fbx");

	return ret;
}

// Update
UPDATE_STATUS M_Scene::Update(float dt)
{
	if (App->debug == true)
	{
		HandleDebugInput();
	}

	for (uint n = 0; n < primitives.size(); n++)
	{
		primitives[n]->Update();
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_STATE::KEY_DOWN)
	{
		App->SaveConfiguration("Resources/Configuration/configuration.JSON");

		//DebugSpawnPrimitive(new Sphere(1.0f, 1.0f));
	}

	return UPDATE_STATUS::CONTINUE;
}

UPDATE_STATUS M_Scene::PostUpdate(float dt)
{
	BROFILER_CATEGORY("M_Scene PostUpdate", Profiler::Color::Yellow)
	
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		if (game_objects[i]->to_delete)
		{
			DeleteGameObject(game_objects[i], i);	
			continue;
		}
		
		if (game_objects[i]->IsActive())
		{
			game_objects[i]->Update();
		}
	}
	
	for (uint n = 0; n < primitives.size(); n++)
	{
		primitives[n]->Render();
	}

	return UPDATE_STATUS::CONTINUE;
}

// Load assets
bool M_Scene::CleanUp()
{
	LOG("Unloading Intro scene");

	for (uint i = 0; i < game_objects.size(); ++i)
	{
		game_objects[i]->CleanUp();
		RELEASE(game_objects[i]);
	}

	game_objects.clear();

	return true;
}

bool M_Scene::LoadConfiguration(Configuration& root)
{
	bool ret = true;

	return ret;
}

bool M_Scene::SaveConfiguration(Configuration& root) const
{
	bool ret = true;

	return ret;
}

// -------------- SCENE METHODS --------------
GameObject* M_Scene::CreateGameObject(const char* name, GameObject* parent)
{
	/*if (NameHasDuplicate(name))
	{
		AddDuplicateNumberToName(game_object);
	}*/
	
	std::string complete_name = name;

	if (!game_objects.empty())
	{
		complete_name += std::to_string(game_objects.size());
	}
	
	GameObject* game_object = new GameObject(game_objects.size(), complete_name);

	if (game_object != nullptr)
	{
		if (game_objects.empty())
		{
			game_object->is_root_object = true;
		}
		
		game_objects.push_back(game_object);

		if (parent != nullptr)
		{
			parent->AddChild(game_object);
		}
	}

	return game_object;
}

void M_Scene::DeleteGameObject(GameObject* game_object, uint index)
{
	if (selected_game_object == game_object)
	{
		selected_game_object = nullptr;
	}
	
	if (!game_objects.empty() && game_object != nullptr)
	{
		if (index != -1)														// If an index was given.
		{
			game_object->CleanUp();												// As it has not been Cleaned Up by its parent, the GameObject needs to call its CleanUp();
			game_objects.erase(game_objects.begin() + index);					// Delete game object at index.
			return;
		}
		else
		{
			for (uint i = 0; i < game_objects.size(); ++i)						// If no index was given.
			{
				if (game_objects[i] == game_object)								// Iterate game_objects until a match is found.
				{
					game_object->CleanUp();
					game_objects.erase(game_objects.begin() + i);				// Delete the game_object at the current loop index.
					return;
				}
			}
		}

		LOG("[ERROR] Could not find game object %s in game_objects vector!", game_object->GetName());
	}
}

bool M_Scene::CreateGameObjectsFromModel(const char* path)
{
	BROFILER_CATEGORY("CreateGameObjectsFromModel()", Profiler::Color::LawnGreen)

	bool ret = false;

	std::string file				= path;															// Getting the file name and the game_object's name.
	uint file_start					= file.find_last_of("//") + 1;									//
	uint file_end					= file.find_last_of(".");										//
	file							= file.substr(file_start, file_end); 							// -------------------------------------------------

	
	std::vector<R_Mesh*> meshes;
	Importer::Meshes::Import(path, meshes);															// Importing the model from the given path. The meshes will be returned in the vector.

	ret = GenerateGameObjectsFromMeshes(path, file, meshes);

	return ret;
}

bool M_Scene::GenerateGameObjectsFromMeshes(const char* path, std::string file_name, std::vector<R_Mesh*>& meshes)
{
	bool ret = true;

	for (uint i = 0; i < meshes.size(); ++i)
	{
		GameObject* game_object = new GameObject();

		GenerateGameObjectComponents(path, file_name, game_object, meshes[i]);						// Will generate the created game_object components with the passed data.

		if (game_object != nullptr)
		{
			if (i == 0)																				// Adding the current game object as a child.
			{																						// 
				root_object->AddChild(game_object);													// Depending on whether it's the first game object of the loop or not, 
				selected_game_object = game_object;													// it will be parented to the Scene's root_object and set as the selected GameObject.
			}																						// 
			else																					// In any other case, the created game object will be set as the child of the parent
			{																						// mesh's GameObject, which will be the selected GameObject.
				selected_game_object->AddChild(game_object);										// 
			}																						// ---------------------------------------------------------

			std::string game_object_name = file_name + "-" + std::to_string(i);						// Adding the index of iteration to the current game object.
			game_object_name += "(" + std::to_string(game_objects.size()) + ")";					//
			game_object->SetName(game_object_name.c_str());											// ---------------------------------------------------------
			
			game_objects.push_back(game_object);													// The newly constructed game_object will be added to the game_objects vector.

			ret = true;
		}
		else																						// The created game object was not valid (nullptr).
		{																							// 
			delete game_object;																		// The game object will be deleted and set to null.
			game_object = nullptr;																	// 
			ret = false;																			// 
		}																							// ------------------------------------------------
	}

	return ret;
}

bool M_Scene::GenerateGameObjectComponents(const char* path, std::string file_name, GameObject* game_object, R_Mesh* mesh)
{
	bool ret = true;
	
	C_Mesh* c_mesh = (C_Mesh*)game_object->CreateComponent(COMPONENT_TYPE::MESH);				// Creating the C_Mesh components of each mesh.
	c_mesh->SetMesh(mesh);																		// The name of the file will be added as the path of the mesh.
	c_mesh->SetMeshPath(file_name.c_str());														// The mesh being iterated will be set as the component's mesh.

	if (!mesh->tex_paths.empty())
	{
		C_Material* c_material = (C_Material*)game_object->CreateComponent(COMPONENT_TYPE::MATERIAL);

		for (uint i = 0; i < mesh->tex_paths.size(); ++i)
		{
			R_Material* material = new R_Material();

			std::string path_str = path;														// Getting the correct path to the texture.
			uint file_start = path_str.find_last_of("/") + 1;									// 
			path_str = path_str.substr(0, file_start);											// 
																								// 
			path_str += mesh->tex_paths[i];														// ----------------------------------------

			Importer::Materials::DevIL::Import(path_str.c_str(), material);						// Importing the data o the texture in the given path.

			if (material->tex_data.id != 0)
			{
				if (c_material->GetMaterial() == nullptr)										// If the component material does not have a R_Material* yet.
				{
					c_material->SetMaterial(material);											// The material that will be used to bind the texture (id, width, height...)
				}

				c_material->textures.push_back(material);										// Vector that will contain all the materials that a single mesh has.
			}
		}
	}
	else
	{
		LOG("[WARNING] Mesh did not have any paths to textures! Mesh path: %s", path);
		ret = false;
	}

	return ret;
}

bool M_Scene::ApplyNewTextureToSelectedGameObject(const char* path)
{
	BROFILER_CATEGORY("ApplyNewTextureToSelectedGameObject()", Profiler::Color::Magenta)
	
	bool ret = true;

	if (selected_game_object != nullptr)
	{
		C_Material* c_material	= selected_game_object->GetMaterialComponent();
		R_Material* material	= new R_Material();

		bool imported = Importer::Materials::DevIL::Import(path, material);										// Should check for duplicates.

		if (imported)																							// If the import was successful
		{
			if (c_material != nullptr)																			// If the C_Material* component already exists.
			{
				c_material->SetMaterial(material);																// Just reset the material with the new one.
				c_material->textures.push_back(material);														// Add the material to the materials loaded in the component.
			}
			else
			{
				c_material = (C_Material*)selected_game_object->CreateComponent(COMPONENT_TYPE::MATERIAL);		// Creates a new C_Material* to accomodate the imported texture.

				if (c_material != nullptr)																		// Checks that the newly created C_Material* is not nullptr.
				{
					c_material->SetMaterial(material);															// 
					c_material->textures.push_back(material);
				}
			}
		}
		else
		{
			LOG("[ERROR] Could not import the dropped texture! Path: %s", path);
		}
	}
	else
	{
		LOG("[ERROR] No game object was being selected! Try again after selecting one from the hierarchy.");
	}

	return ret;
}

GameObject* M_Scene::GetRootGameObject() const
{
	return root_object;
}

void M_Scene::SetRootGameObject(GameObject* game_object)
{
	if (game_object != root_object)
	{
		root_object = game_object;
	}
}

void M_Scene::ChangeSceneName(const char* name)
{
	root_object->SetName(name);
}

GameObject* M_Scene::GetSelectedGameObject() const
{
	return selected_game_object;
}

void M_Scene::SetSelectedGameObject(GameObject* game_object)
{
	if (game_object != selected_game_object)
	{
		selected_game_object = game_object;
	}
}

void M_Scene::DeleteSelectedGameObject()
{
	if (selected_game_object != nullptr)
	{
		DeleteGameObject(selected_game_object);
		//selected_game_object = nullptr;
	}
}

/*bool M_Scene::NameHasDuplicate(const char* name)
{
	uint duplicates = 0;
	
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		if (strcmp(name, game_objects[i]->GetName()) == 0)
		{
			return true;
			++duplicates;
		}
	}

	return duplicates;

	return false;
}*/

void M_Scene::HandleDebugInput()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_STATE::KEY_DOWN)
	{
		//DebugSpawnPrimitive(new Sphere(1.0f, 12, 24));
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_STATE::KEY_DOWN)
	{
		DebugSpawnPrimitive(new P_Cube());
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_STATE::KEY_DOWN)
	{
		//DebugSpawnPrimitive(new Cylinder());
	}

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_STATE::KEY_DOWN)
	{
		for (uint n = 0; n < primitives.size(); n++)
		{
			primitives[n]->SetPos((float)(std::rand() % 40 - 20), 10.f, (float)(std::rand() % 40 - 20));
		}
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_STATE::KEY_DOWN)
	{
		//Get a vector indicating the direction from the camera viewpoint to the "mouse"
		float mouse_x_position = ((float)App->input->GetMouseX() / (float)App->window->GetWidth()) * 2.f - 1.f;
		float mouse_y_position = -((float)App->input->GetMouseY() / (float)App->window->GetHeight()) * 2.f + 1.f;

		const vec2 mousePos(mouse_x_position, mouse_y_position);

		const vec4 rayEye = inverse(App->renderer->ProjectionMatrix) * vec4(mousePos.x, mousePos.y, -1.f, 1.f);
		const vec4 rayWorld(inverse(App->camera->GetViewMatrix()) * vec4(rayEye.x, rayEye.y, -1.f, 0.f));

		vec3 Dir(rayWorld.x, rayWorld.y, rayWorld.z);
	}
}

void M_Scene::DebugSpawnPrimitive(Primitive* p)
{
	primitives.push_back(p);
	p->SetPos(App->camera->position.x, App->camera->position.y, App->camera->position.z);
}
#include "Profiler.h"
#include "MathStructures.h"
#include "glmath.h"																// WAITING UNTIL FRUSTUM TO DELETE IT

#include "Application.h"														// ATTENTION: Globals.h already included in Module.h
#include "M_Window.h"
#include "M_Camera3D.h"
#include "M_Renderer3D.h"
#include "M_Input.h"
#include "M_Editor.h"
#include "Primitive.h"

#include "I_Scenes.h"
#include "I_Meshes.h"
#include "I_Textures.h"
#include "R_Mesh.h"
#include "R_Texture.h"

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

	App->camera->LookAt(vec3(0.0f, 0.0f, 0.0f) /*float3::zero*/);

	if (root_object == nullptr)
	{
		root_object = CreateGameObject("Main Scene");

		selected_game_object = root_object;
	}

	ImportScene("Assets/Models/street/Street Environment_V01.FBX");

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
		game_object->CleanUp();													// As it has not been Cleaned Up by its parent, the GameObject needs to call its CleanUp();
		
		if (index != -1)														// If an index was given.
		{
			game_objects.erase(game_objects.begin() + index);					// Delete game object at index.
			return;
		}
		else
		{
			for (uint i = 0; i < game_objects.size(); ++i)						// If no index was given.
			{
				if (game_objects[i] == game_object)								// Iterate game_objects until a match is found.
				{
					game_objects.erase(game_objects.begin() + i);				// Delete the game_object at the current loop index.
					return;
				}
			}
		}

		LOG("[ERROR] Could not find game object %s in game_objects vector!", game_object->GetName());
	}
}

bool M_Scene::ImportScene(const char* path)
{
	BROFILER_CATEGORY("M_Scene::ImportScene()", Profiler::Color::LawnGreen);

	bool ret = true;

	Importer::Scenes::Import(path, game_objects);													// Importing the new game objects directly into the game_objects vector.

	//std::vector<GameObject*> nodes;																	// Creating an step in the middle of the import to manipulate the
	//Importer::Scenes::Import(path, nodes);															// game objects created by the Import Pipeline.

	//for (uint i = 0; i < nodes.size(); ++i)
	//{	
	//	game_objects.push_back(nodes[i]);
	//}
	//
	//nodes.clear();

	return ret;
}

bool M_Scene::ApplyNewTextureToSelectedGameObject(const char* path)
{
	BROFILER_CATEGORY("ApplyNewTextureToSelectedGameObject()", Profiler::Color::Magenta)
	
	bool ret = true;

	if (selected_game_object != nullptr)
	{
		C_Material* c_material	= selected_game_object->GetMaterialComponent();									// GetMaterialComponent() will return nullptr if C_Material* does not exist.
		R_Texture* texture		= new R_Texture();

		bool success = Importer::Textures::Import(path, texture);												// Should check for duplicates.

		if (success)																							// If the import was successful
		{
			if (c_material != nullptr)																			// If the C_Material* component already exists.
			{
				c_material->SetTexture(texture);																// Just reset the material with the new one.
				c_material->textures.push_back(texture);														// Add the material to the materials loaded in the component.
			}
			else
			{
				c_material = (C_Material*)selected_game_object->CreateComponent(COMPONENT_TYPE::MATERIAL);		// Creates a new C_Material* to accomodate the imported texture.

				if (c_material != nullptr)																		// Checks that the newly created C_Material* is not nullptr.
				{
					c_material->SetTexture(texture);															// Sets the new C_Material component with the r_material with the imported tex.
					c_material->textures.push_back(texture);
				}
			}
		}
		else
		{
			delete texture;
			texture = nullptr;
			
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

		float3 go_ref = game_object->GetTransformComponent()->GetWorldPosition();
		vec3 reference = { go_ref.x, go_ref.y, go_ref.z };

		App->camera->SetReference(reference);
	}
}

void M_Scene::DeleteSelectedGameObject()
{
	if (selected_game_object != nullptr)
	{
		DeleteGameObject(selected_game_object);
	}
}

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

		/*const float2 mouse_pos(mouse_x_position, mouse_y_position);

		const float4 ray_eye = App->renderer->GetProjectionMatrix().Inverted() * float4(mouse_pos.x, mouse_pos.y, -1.f, 1.f);
		const float4 ray_world(App->camera->GetViewMatrix().Inverted() * float4(ray_eye.x, ray_eye.y, -1.f, 0.f));*/

		const vec2 mouse_pos(mouse_x_position, mouse_y_position);

		const vec4 ray_eye = inverse(App->renderer->GetProjectionMatrix()) * vec4(mouse_pos.x, mouse_pos.y, -1.f, 1.f);
		const vec4 ray_world(inverse(App->camera->GetViewMatrix()) * vec4(ray_eye.x, ray_eye.y, -1.f, 0.f));

		float3 dir(ray_world.x, ray_world.y, ray_world.z);
	}
}

void M_Scene::DebugSpawnPrimitive(Primitive* p)
{
	primitives.push_back(p);
	p->SetPos(App->camera->position.x, App->camera->position.y, App->camera->position.z);
}
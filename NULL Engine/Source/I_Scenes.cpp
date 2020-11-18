// ----------------------------------------------------
// I_Scenes.cpp --- Importing .FBX files into scenes.
// Interface class between the Engine and Assimp.
// ----------------------------------------------------

#include <string>

#include "Assimp.h"

#include "VarTypedefs.h"
#include "Log.h"

#include "Application.h"
#include "M_FileSystem.h"
#include "M_Scene.h"												// TMP

#include "Resource.h"												// See if they can be delegated to the other importers.
#include "R_Mesh.h"													// 
#include "R_Material.h"												// 
#include "R_Texture.h"												// ----------------------------------------------------

#include "GameObject.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"

#include "I_Meshes.h"
#include "I_Materials.h"
#include "I_Textures.h"

#include "I_Scenes.h"

#include "MemoryManager.h"

#pragma comment (lib, "Source/Dependencies/Assimp/libx86/assimp.lib")

using namespace Importer::Scenes;																		// Not a good thing to do but it will be employed sparsely and only inside this .cpp

void Importer::Scenes::Import(const char* path, std::vector<GameObject*>& game_object_nodes)
{
	const char* scene_path = path;
	
	LOG("[STATUS] Importing Scene: %s", App->file_system->GetFileAndExtension(path).c_str());

	char* buffer	= nullptr;
	uint file_size	= App->file_system->Load(path, &buffer);

	if (file_size > 0)
	{
		const aiScene* ai_scene = aiImportFileFromMemory(buffer, file_size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);

		if (ai_scene == nullptr || ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !ai_scene->mRootNode)
		{
			LOG("[ERROR] Error loading scene %s", aiGetErrorString());

			return;
		}

		Utilities::ProcessNode(path, ai_scene, ai_scene->mRootNode, game_object_nodes, App->scene->GetRootGameObject());
	}
}

void Importer::Scenes::Utilities::ProcessNode(const char* scene_path, const aiScene* ai_scene, const aiNode* ai_node, std::vector<GameObject*>& game_objects, GameObject* parent)
{
	std::string scene_file = App->file_system->GetFileAndExtension(scene_path);
	
	GameObject* game_object = new GameObject();
	
	if (ai_node == ai_scene->mRootNode)
	{
		game_object->SetName(scene_file.c_str());
	}
	else
	{
		game_object->SetName(ai_node->mName.C_Str());
	}
	
	game_object->parent = parent;

	if (parent != nullptr)
	{
		parent->AddChild(game_object);
	}

	Utilities::ImportTransform(ai_node, game_object);														// Load Transforms
	Utilities::ImportMeshes(scene_file.c_str(), ai_scene, ai_node, game_object);							// Load Meshes
	//Utilities::ImportMaterials(scene_path, game_object);													// Load Materials
	Utilities::ImportMaterials(ai_scene, ai_node, game_object);												// Load Materials

	game_objects.push_back(game_object);

	for (uint i = 0; i < ai_node->mNumChildren; ++i)
	{
		Utilities::ProcessNode(scene_path, ai_scene, ai_node->mChildren[i], game_objects, game_object);
	}

	scene_file.clear();
}

void Importer::Scenes::Utilities::ImportTransform(const aiNode* ai_node, GameObject* game_object)
{
	aiTransform ai_t;																						// Transform structure for Assimp. aiVector3D and aiQuaternion.
	Transform	ma_t;																						// Transform structure for MathGeoLib. float3 and Quat.

	ai_node->mTransformation.Decompose(ai_t.scale, ai_t.rotation, ai_t.position);							// --- Getting the Transform stored in the node.

	ma_t.position	= { ai_t.position.x, ai_t.position.y, ai_t.position.z };								// 
	ma_t.rotation	= { ai_t.rotation.x, ai_t.rotation.y, ai_t.rotation.z, ai_t.rotation.w };				// 
	ma_t.scale		= { ai_t.scale.x, ai_t.scale.y, ai_t.scale.z };											// ---------------------------------------------

	std::string root_node = ai_node->mName.C_Str();															// Will be used for LOG Debug.
	std::string node_name = ai_node->mName.C_Str();															// Assimp generates dummy nodes to store multiple FBX transformations.
	bool found_dummy_node = true;																			// All those transformations will be collapsed to the first non_dummy node.
	while (found_dummy_node) 
	{
		found_dummy_node = false;

		if (node_name.find("_$AssimpFbx$_") != std::string::npos && ai_node->mNumChildren == 1)				// All dummy nodes will contain the "_$AssimpFbx$_" string and only one child node.
		{	
			ai_node = ai_node->mChildren[0];																// As dummies will only have one child, selecting the next one to process is easy.

			ai_node->mTransformation.Decompose(ai_t.scale, ai_t.rotation, ai_t.position);					// --- Getting the Transform stored in the dummy node.

			Transform dummy;																				// 
			dummy.position	= { ai_t.position.x, ai_t.position.y, ai_t.position.z };						// 
			dummy.rotation	= { ai_t.rotation.x, ai_t.rotation.y, ai_t.rotation.z, ai_t.rotation.w };		// 
			dummy.scale		= { ai_t.scale.x, ai_t.scale.y, ai_t.scale.z };									// ---------------------------------------------------

			ma_t.position.Add(dummy.position);																// --- Adding the dummy's Transform to the current one.
			ma_t.rotation.Mul(dummy.rotation);																// 
			ma_t.scale.Mul(dummy.scale);																	// ----------------------------------------------------

			node_name			= ai_node->mName.C_Str();													// Setting the next node name to check with the name of the dummy node.
			found_dummy_node	= true;																		// Setting it to true so the loop is run for another iteration.

			game_object->is_dummy = true;
		}
	}

	game_object->GetTransformComponent()->ImportTransform(ma_t.position, ma_t.rotation, ma_t.scale);		// Importing the final Transform into the game object's transform component.

	LOG("[IMPORTER] Imported the transforms of node: %s", root_node.c_str());
}

void Importer::Scenes::Utilities::ImportMeshes(const char* scene_file, const aiScene* ai_scene, const aiNode* ai_node, GameObject* game_object)
{
	std::vector<R_Mesh*> meshes;
	Importer::Meshes::Utilities::GetMeshesFromNode(ai_scene, ai_node, meshes);									//
	
	for (uint i = 0; i < meshes.size(); ++i)																	//
	{
		C_Mesh* c_mesh = (C_Mesh*)game_object->CreateComponent(COMPONENT_TYPE::MESH);							//

		c_mesh->SetMesh(meshes[i]);																				// 
		c_mesh->SetMeshPath(scene_file);																		// 

		//Utilities::ImportMaterials(scene_path, game_object, meshes[i]);										// 
	}

	meshes.clear();

	LOG("[IMPORTER] Imported the meshes of node: %s", ai_node->mName.C_Str());
}

void Importer::Scenes::Utilities::ImportMaterials(const char* scene_path, GameObject* game_object)
{
	std::vector<C_Mesh*> c_meshes = game_object->GetAllMeshComponents();										// 

	for (uint i = 0; i < c_meshes.size(); ++i)
	{
		C_Material* c_material	= (C_Material*)game_object->CreateComponent(COMPONENT_TYPE::MATERIAL);			// For now, creating more than one material component is not allowed.

		if (c_material != nullptr)
		{
			R_Mesh* r_mesh = c_meshes[i]->GetMesh();															// 
			for (uint j = 0; j < r_mesh->tex_paths.size(); ++j)
			{
				R_Material* r_material	= new R_Material();														//
				R_Texture* r_texture = new R_Texture();															// 

				std::string dir_path;																			//
				std::string file_name;
				App->file_system->SplitFilePath(scene_path, &dir_path, &file_name, nullptr);					//
				dir_path += r_mesh->tex_paths[j];																//

				std::string path_to_tex = ASSET_TEXTURES_PATH;
				path_to_tex += file_name;

				//Importer::Materials::Import();
				
				/*Importer::Materials::Import(dir_path.c_str(), r_material);									//
																												
				if (r_material != nullptr && r_material->tex_data.id != 0)										//
				{
					if (c_material->GetMaterial() == nullptr)													//
					{
						c_material->SetMaterial(r_material);													//
					}

					c_material->textures.push_back(r_material);													//
				}*/

				uint tex_id = Importer::Textures::Import(dir_path.c_str(), r_texture);							//

				if (tex_id != 0 && r_texture != nullptr)														//
				{
					if (c_material->GetTexture() == nullptr)													//
					{
						c_material->SetTexture(r_texture);														//
					}

					c_material->textures.push_back(r_texture);													//
				}
			}
		}
	}
}

void Importer::Scenes::Utilities::ImportMaterials(const aiScene* ai_scene, const aiNode* ai_node, GameObject* game_object)
{
	for (uint i = 0; i < ai_node->mNumMeshes; ++i)
	{
		aiMesh* ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[i]];												// aiMesh_array[index].


	}
}

void Importer::Scenes::Save(const aiScene* ai_scene, char** buffer)
{

}

void Importer::Scenes::Load(const char* buffer, aiScene* ai_scene)
{

}
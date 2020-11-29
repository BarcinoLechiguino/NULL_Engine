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
#include "M_Scene.h"															// TMP

#include "Resource.h"															// See if they can be delegated to the other importers.
#include "R_Mesh.h"																// 
#include "R_Material.h"															// 
#include "R_Texture.h"															// ----------------------------------------------------

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

//typedef std::vector<uint> uintVec;
//typedef std::vector<GameObject*> GOVec;

using namespace Importer::Scenes;																		// Not a good thing to do but it will be employed sparsely and only inside this .cpp

uint64 Importer::Scenes::Save(const aiScene* ai_scene, char** buffer)
{
	uint64 size = 0;
	


	return size;
}

void Importer::Scenes::Load(const char* buffer, aiScene* ai_scene)
{

}

void Importer::Scenes::Import(const char* path, std::vector<GameObject*>& game_objects)
{	
	bool ret = true;
	
	// Load .meta and check whether or not the model/scene has not already been loaded.

	if (ret)																									// If .meta does not exist or the file IDs are incorrect, import from Assets.
	{
		Utilities::ImportFromAssets(path, game_objects);
	}
	else
	{
		Utilities::ImportFromLibrary(path, game_objects);														// If it has been already loaded, load the scene from the Library.
	}
}

void Importer::Scenes::Utilities::ImportFromLibrary(const char* path, std::vector<GameObject*>& game_objects)
{

}

void Importer::Scenes::Utilities::ImportFromAssets(const char* path, std::vector<GameObject*>& game_objects)
{
	LOG("[STATUS] Importing Scene: %s", App->file_system->GetFileAndExtension(path).c_str());

	char* buffer = nullptr;
	uint file_size = App->file_system->Load(path, &buffer);

	if (file_size > 0)
	{
		const aiScene* ai_scene = aiImportFileFromMemory(buffer, file_size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);

		if (ai_scene == nullptr || ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !ai_scene->mRootNode)
		{
			LOG("[ERROR] Error loading scene %s", aiGetErrorString());
			return;
		}

		Utilities::ProcessNode(path, ai_scene, ai_scene->mRootNode, game_objects, App->scene->GetRootGameObject());
	}
}

void Importer::Scenes::Utilities::ProcessNode(const char* scene_path, const aiScene* ai_scene, const aiNode* ai_node, std::vector<GameObject*>& game_objects, GameObject* parent)
{
	std::string scene_file = App->file_system->GetFileAndExtension(scene_path);
	
	GameObject* game_object = new GameObject();

	ai_node = Utilities::ImportTransform(ai_node, game_object);												// Load Transforms. Returns the first non-dummy node that is found.
	Utilities::ImportMeshesAndMaterials(scene_path, ai_scene, ai_node, game_object);
	
	const char* node_name = (ai_node == ai_scene->mRootNode) ? scene_file.c_str() : ai_node->mName.C_Str();	// If node is root node, use the scene file's name. Else use the node's name.
	game_object->SetName(node_name);

	if (parent != nullptr)
	{
		game_object->parent = parent;
		parent->AddChild(game_object);
	}
	
	game_objects.push_back(game_object);

	for (uint i = 0; i < ai_node->mNumChildren; ++i)
	{
		Utilities::ProcessNode(scene_path, ai_scene, ai_node->mChildren[i], game_objects, game_object);
	}

	scene_file.clear();
}

const aiNode* Importer::Scenes::Utilities::ImportTransform(const aiNode* ai_node, GameObject* game_object)
{
	// Assimp generates dummy nodes to store multiple FBX transformations.
	// All those transformations will be collapsed to the first non-dummy node.
	
	aiTransform ai_t;																						// Transform structure for Assimp. aiVector3D and aiQuaternion.
	Transform	ma_t;																						// Transform structure for MathGeoLib. float3 and Quat.

	ai_node->mTransformation.Decompose(ai_t.scale, ai_t.rotation, ai_t.position);							// --- Getting the Transform stored in the node.

	ma_t.position	= { ai_t.position.x, ai_t.position.y, ai_t.position.z };								// 
	ma_t.rotation	= { ai_t.rotation.x, ai_t.rotation.y, ai_t.rotation.z, ai_t.rotation.w };				// 
	ma_t.scale		= { ai_t.scale.x, ai_t.scale.y, ai_t.scale.z };											// ---------------------------------------------

	while (NodeIsDummyNode(*ai_node))																		// All dummy nodes will contain the "_$AssimpFbx$_" string and only one child node.
	{
		ai_node = ai_node->mChildren[0];																	// As dummies will only have one child, selecting the next one to process is easy.

		ai_node->mTransformation.Decompose(ai_t.scale, ai_t.rotation, ai_t.position);						// --- Getting the Transform stored in the dummy node.

		Transform dummy;																					// 
		dummy.position	= { ai_t.position.x, ai_t.position.y, ai_t.position.z };							// 
		dummy.rotation	= { ai_t.rotation.x, ai_t.rotation.y, ai_t.rotation.z, ai_t.rotation.w };			// 
		dummy.scale		= { ai_t.scale.x, ai_t.scale.y, ai_t.scale.z };										// ---------------------------------------------------

		ma_t.position.Add(dummy.position);																	// --- Adding the dummy's Transform to the current one.
		ma_t.rotation.Mul(dummy.rotation);																	// 
		ma_t.scale.Mul(dummy.scale);																		// ----------------------------------------------------
	}
	
	game_object->GetTransformComponent()->ImportTransform(ma_t.position, ma_t.rotation, ma_t.scale);		// Importing the final Transform into the game object's transform component.

	LOG("[IMPORTER] Imported the transforms of node: %s", ai_node->mName.C_Str());

	return ai_node;
}

void Importer::Scenes::Utilities::ImportMeshesAndMaterials(const char* path, const aiScene* ai_scene, const aiNode* ai_node, GameObject* game_object)
{
	for (uint i = 0; i < ai_node->mNumMeshes; ++i)
	{
		aiMesh* ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[i]];

		if (ai_mesh != nullptr && ai_mesh->HasFaces())
		{
			Importer::Scenes::Utilities::ImportMesh(path, ai_mesh, game_object);

			if (ai_mesh->mMaterialIndex >= 0)
			{
				aiMaterial* ai_material = ai_scene->mMaterials[ai_mesh->mMaterialIndex];
				
				Importer::Scenes::Utilities::ImportMaterial(path, ai_material, game_object);
			}
		}
	}   
}

void Importer::Scenes::Utilities::ImportMesh(const char* path, const aiMesh* ai_mesh, GameObject* game_object)
{
	R_Mesh* r_mesh = new R_Mesh();

	r_mesh->SetAssetsPath(path);
	r_mesh->SetAssetsFile(App->file_system->GetFileAndExtension(path).c_str());

	Importer::Meshes::Import(ai_mesh, r_mesh);

	if (r_mesh != nullptr)
	{
		C_Mesh* c_mesh = (C_Mesh*)game_object->CreateComponent(COMPONENT_TYPE::MESH);

		c_mesh->SetMesh(r_mesh);
		c_mesh->SetMeshPath(path);
	}
	else
	{
		delete r_mesh;
		r_mesh = nullptr;
	}
}

void Importer::Scenes::Utilities::ImportMaterial(const char* path, const aiMaterial* ai_material, GameObject* game_object)
{
	R_Material* r_material	= new R_Material();														// Only considering one texture per mesh.
	R_Texture* r_texture	= new R_Texture();

	Importer::Materials::Import(path, ai_material, r_material, r_texture);

	C_Material* c_material = (C_Material*)game_object->CreateComponent(COMPONENT_TYPE::MATERIAL);

	if (c_material != nullptr)
	{
		c_material->SetMaterial(r_material);														// C_Material will always have a R_Material*.

		if (r_texture->GetTextureID() != 0)
		{
			c_material->SetTexture(r_texture);
		}
		else
		{
			delete r_texture;
			r_texture = nullptr;

			LOG("[IMPORTER] Imported aiMaterial had no texture!");
		}
	}
	else
	{
		delete r_material;
		r_material = nullptr;

		delete r_texture;
		r_texture = nullptr;

		LOG("[ERROR] Importer: Could not create a Material Component for %s!", game_object->GetName());
	}
}

bool Importer::Scenes::Utilities::NodeIsDummyNode(const aiNode& ai_node)
{
	return (strstr(ai_node.mName.C_Str(), "_$AssimpFbx$_") != nullptr && ai_node.mNumChildren == 1);		// All dummy nodes will contain the "_$AssimpFbx$_" string and only one child node.
}
// ----------------------------------------------------
// I_Scenes.cpp --- Importing .FBX files into scenes.
// Interface class between the Engine and Assimp.
// ----------------------------------------------------

#include <string>

#include "Assimp.h"
#include "JSONParser.h"

#include "VariableTypedefs.h"
#include "Log.h"

#include "Random.h"

#include "Application.h"
#include "M_FileSystem.h"
#include "M_ResourceManager.h"

#include "Resource.h"															// See if they can be delegated to the other importers.
#include "R_Model.h"															// 
#include "R_Mesh.h"																// 
#include "R_Material.h"															// 
#include "R_Texture.h"															// ----------------------------------------------------

#include "I_Meshes.h"
#include "I_Materials.h"
#include "I_Textures.h"

#include "I_Scenes.h"

#include "MemoryManager.h"

#pragma comment (lib, "Source/Dependencies/Assimp/libx86/assimp.lib")

using namespace Importer::Scenes;																		// Not a good thing to do but it will be employed sparsely and only inside this .cpp

uint64 Importer::Scenes::Save(const R_Model* r_model, char** buffer)
{
	uint64 size = 0;



	return size;
}

void Importer::Scenes::Load(const char* buffer, R_Model* r_model)
{

}

void Importer::Scenes::Import(const char* path, R_Model* r_model)
{
	bool meta_is_updated = false;

	// Get Meta File and Check it is Updated

	if (meta_is_updated)
	{
		Utilities::ImportFromLibrary(path, r_model);															// Make this processing go through the Resource Manager.
	}
	else
	{
		Utilities::ImportFromAssets(path, r_model);
	}
}

void Importer::Scenes::Utilities::ImportFromLibrary(const char* path, R_Model* r_model)
{

}

void Importer::Scenes::Utilities::ImportFromAssets(const char* path, R_Model* r_model)
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

		Utilities::ProcessNode(path, ai_scene, ai_scene->mRootNode, r_model, ModelNode());							// First Parent is empty. Later assigned to scene_root.
	}
}

void Importer::Scenes::Utilities::ProcessNode(const char* scene_path, const aiScene* ai_scene, const aiNode* ai_node, R_Model* r_model, const ModelNode& parent)
{
	std::string scene_file = App->file_system->GetFileAndExtension(scene_path);

	ModelNode model_node	= ModelNode();
	model_node.name			= (ai_node == ai_scene->mRootNode) ? scene_file.c_str() : ai_node->mName.C_Str();
	model_node.uid			= Random::LCG::GetRandomUint();
	model_node.parent_uid	= parent.uid;

	ai_node = Utilities::ImportTransform(ai_node, model_node);
	Utilities::ImportMeshesAndMaterials(scene_path, ai_scene, ai_node, model_node);

	r_model->model_nodes.push_back(model_node);

	for (uint i = 0; i < ai_node->mNumChildren; ++i)
	{
		ProcessNode(scene_path, ai_scene, ai_node->mChildren[i], r_model, model_node);
	}

	scene_file.clear();
}

const aiNode* Importer::Scenes::Utilities::ImportTransform(const aiNode* ai_node, ModelNode& model_node)
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
	
	model_node.transform = ma_t;

	LOG("[IMPORTER] Imported the transforms of node: %s", model_node.name.c_str());

	return ai_node;
}

void Importer::Scenes::Utilities::ImportMeshesAndMaterials(const char* path, const aiScene* ai_scene, const aiNode* ai_node, ModelNode& model_node)
{
	const char* node_name = ai_node->mName.C_Str();

	for (uint i = 0; i < ai_node->mNumMeshes; ++i)
	{
		aiMesh* ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[i]];

		if (ai_mesh != nullptr && ai_mesh->HasFaces())
		{
			Importer::Scenes::Utilities::ImportMesh(path, node_name, ai_mesh, model_node);

			if (ai_mesh->mMaterialIndex >= 0)
			{
				aiMaterial* ai_material = ai_scene->mMaterials[ai_mesh->mMaterialIndex];

				Importer::Scenes::Utilities::ImportMaterial(path, node_name, ai_material, model_node);
			}
		}
	}
}

void Importer::Scenes::Utilities::ImportMesh(const char* path, const char* name, const aiMesh* ai_mesh, ModelNode& model_node)
{
	R_Mesh* r_mesh = new R_Mesh();

	std::string assets_file = std::string(name) + std::string(MESHES_EXTENSION);									// The name of the mesh will be stored in ai_node->mName.C_Str();
	r_mesh->SetAssetsPath(path);
	r_mesh->SetAssetsFile(assets_file.c_str());
	assets_file.clear();

	Importer::Meshes::Import(ai_mesh, r_mesh);

	if (r_mesh != nullptr)
	{
		r_mesh->SetLibraryPathAndFile();
		App->resource_manager->AddResource(r_mesh);

		model_node.mesh_uid = r_mesh->GetUID();
	}
	else
	{
		RELEASE(r_mesh);
	}
}

void Importer::Scenes::Utilities::ImportMaterial(const char* path, const char* name, const aiMaterial* ai_material, ModelNode& model_node)
{
	R_Material* r_material	= new R_Material();																	// Only considering one texture per mesh.
	R_Texture* r_texture	= new R_Texture();

	std::string assets_file = std::string(name) + MATERIALS_EXTENSION;
	r_material->SetAssetsPath(path);
	r_material->SetAssetsFile(assets_file.c_str());
	assets_file.clear();

	Importer::Materials::Import(path, ai_material, r_material, r_texture);

	if (r_material == nullptr)																					// Checking that the resources are valid.
	{																											// 
		//LOG("[ERROR] Importer: Could not create a Material Component for %s!", game_object->GetName());		// 
		r_material->CleanUp();																					// 
		r_texture->CleanUp();																					// 
		RELEASE(r_material);																					// 
		RELEASE(r_texture);																						// 
		return;																									// 
	}																											// 
																												// 
	if (r_texture->GetTextureID() == 0)																			// 
	{																											// 
		r_texture->CleanUp();																					// 
		RELEASE(r_texture);																						// 
		LOG("[IMPORTER] Imported aiMaterial had no texture!");													// 
	}																											// --------------------------------------

	r_material->SetLibraryPathAndFile();																		// Adding the resources to the resource manager's maps
	App->resource_manager->AddResource(r_material);																//
	model_node.material_uid = r_material->GetUID();																//
	if (r_texture != nullptr)																					// 
	{																											// 
		App->resource_manager->AddResource(r_texture);															// 
		model_node.texture_uid = r_texture->GetUID();															//
	}																											// ----------------------------------------------------																										// -------------------------------------------------------------------
}

bool Importer::Scenes::Utilities::NodeIsDummyNode(const aiNode& ai_node)
{
	return (strstr(ai_node.mName.C_Str(), "_$AssimpFbx$_") != nullptr && ai_node.mNumChildren == 1);	// All dummy nodes will contain the "_$AssimpFbx$_" string and only one child node.
}
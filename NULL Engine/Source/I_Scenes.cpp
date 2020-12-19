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

void Importer::Scenes::Import(const char* buffer, uint size, R_Model* r_model)
{
	LOG("[STATUS] Importing Scene: %s", r_model->GetAssetsFile());

	const aiScene* ai_scene = aiImportFileFromMemory(buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);

	if (ai_scene == nullptr || ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !ai_scene->mRootNode)
	{
		LOG("[ERROR] Error loading scene %s", aiGetErrorString());
		return;
	}

	Utilities::ProcessNode(ai_scene, ai_scene->mRootNode, r_model, ModelNode());							// First Parent is empty. Later assigned to scene_root.
}

void Importer::Scenes::Utilities::ProcessNode(const aiScene* ai_scene, const aiNode* ai_node, R_Model* r_model, const ModelNode& parent)
{
	ModelNode model_node	= ModelNode();
	model_node.uid			= Random::LCG::GetRandomUint();
	model_node.parent_uid	= parent.uid;

	ai_node = Utilities::ImportTransform(ai_node, model_node);
	Utilities::ImportMeshesAndMaterials(ai_scene, ai_node, r_model, model_node);

	model_node.name			= (ai_node == ai_scene->mRootNode) ? r_model->GetAssetsFile() : ai_node->mName.C_Str();

	r_model->model_nodes.push_back(model_node);

	for (uint i = 0; i < ai_node->mNumChildren; ++i)
	{
		ProcessNode(ai_scene, ai_node->mChildren[i], r_model, model_node);
	}
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

		/*ma_t.position	= */ma_t.position.Add(dummy.position);												// --- Adding the dummy's Transform to the current one.
		/*ma_t.rotation	= */ma_t.rotation.Mul(dummy.rotation);												// 
		/*ma_t.scale	= */ma_t.scale.Mul(dummy.scale);													// ----------------------------------------------------
	}
	
	model_node.transform	= ma_t;

	LOG("[IMPORTER] Imported the transforms of node: %s", ai_node->mName.C_Str());

	return ai_node;
}

void Importer::Scenes::Utilities::ImportMeshesAndMaterials(const aiScene* ai_scene, const aiNode* ai_node, R_Model* r_model, ModelNode& model_node)
{
	const char* node_name = ai_node->mName.C_Str();

	for (uint i = 0; i < ai_node->mNumMeshes; ++i)
	{
		aiMesh* ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[i]];

		if (ai_mesh != nullptr && ai_mesh->HasFaces())
		{
			Importer::Scenes::Utilities::ImportMesh(node_name, ai_mesh, model_node);

			if (ai_mesh->mMaterialIndex >= 0)
			{
				aiMaterial* ai_material = ai_scene->mMaterials[ai_mesh->mMaterialIndex];

				Importer::Scenes::Utilities::ImportMaterial(node_name, ai_material, r_model, model_node);
			}
		}
	}
}

void Importer::Scenes::Utilities::ImportMesh(const char* node_name, const aiMesh* ai_mesh, ModelNode& model_node)
{
	std::string assets_path = ASSETS_MODELS_PATH + std::string(node_name) + MESHES_EXTENSION;						// As meshes are contained in models, the assets path is kind of made-up.
	R_Mesh* r_mesh = (R_Mesh*)App->resource_manager->CreateResource(RESOURCE_TYPE::MESH, assets_path.c_str());
	assets_path.clear();

	Importer::Meshes::Import(ai_mesh, r_mesh);

	if (r_mesh != nullptr)
	{ 
		model_node.mesh_uid = r_mesh->GetUID();
	}
	else
	{
		App->resource_manager->DeleteResource(r_mesh->GetUID());
	}
}

void Importer::Scenes::Utilities::ImportMaterial(const char* node_name, const aiMaterial* ai_material, R_Model* r_model, ModelNode& model_node)
{
	std::string mat_full_path	= App->file_system->GetDirectory(r_model->GetAssetsPath()) + node_name + MATERIALS_EXTENSION;
	R_Material* r_material		= (R_Material*)App->resource_manager->CreateResource(RESOURCE_TYPE::MATERIAL, mat_full_path.c_str());		// Only considering one texture per mesh.
	mat_full_path.clear();

	if (r_material == nullptr)
	{
		return;
	}
	
	Importer::Materials::Import(ai_material, r_material);

	model_node.material_uid = r_material->GetUID();																		//

	for (uint i = 0; i < r_material->materials.size(); ++i)
	{
		Material material = r_material->materials[i];																	//
		
		const char* tex_path	= material.texture_assets_path.c_str();
		char* buffer			= nullptr;
		uint read				= App->file_system->Load(tex_path, &buffer);
		if (buffer != nullptr && read > 0)
		{
			R_Texture* r_texture = (R_Texture*)App->resource_manager->CreateResource(RESOURCE_TYPE::TEXTURE, tex_path);
			Importer::Textures::Import(buffer, read, r_texture);																//

			if (r_texture->GetTextureID() == 0)
			{
				App->resource_manager->DeleteResource(r_texture->GetUID());
				RELEASE_ARRAY(buffer);
				continue;
			}

			if (material.type == TEXTURE_TYPE::DIFFUSE)																	// For now only the diffuse texture will be used on models' meshes.
			{
				model_node.texture_uid = r_texture->GetUID();
			}

			//RELEASE_ARRAY(buffer);																					// TMP Commented. MMGR breaks here
		}
		else
		{
			LOG("[ERROR] Importer: Could not load texture from given path! Path: %s", tex_path);
		}
	}
}

bool Importer::Scenes::Utilities::NodeIsDummyNode(const aiNode& ai_node)
{
	return (strstr(ai_node.mName.C_Str(), "_$AssimpFbx$_") != nullptr && ai_node.mNumChildren == 1);	// All dummy nodes will contain the "_$AssimpFbx$_" string and only one child node.
}
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

#include "GameObject.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"

#include "I_Meshes.h"
#include "I_Materials.h"

#include "I_Scenes.h"

#include "MemoryManager.h"

#pragma comment (lib, "Source/Dependencies/Assimp/libx86/assimp.lib")

using namespace Importer::Scenes;																		// Not a good thing to do but it will be employed sparsely and only inside this .cpp

void Importer::Scenes::Import(const aiScene* ai_scene)
{
	if (ai_scene == nullptr || ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !ai_scene->mRootNode)
	{
		LOG("[ERROR] Error loading scene %s", aiGetErrorString());

		return;
	}

	//Utilities::ProcessNode(ai_scene, ai_scene->mRootNode);
}

void Importer::Scenes::Import(const char* path, std::vector<GameObject*>& game_object_nodes)
{
	//const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

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

		Utilities::ProcessNode(ai_scene, ai_scene->mRootNode, game_object_nodes);
	}
}

void Importer::Scenes::Utilities::ProcessNode(const aiScene* ai_scene, const aiNode* ai_node, std::vector<GameObject*>& game_object_nodes)
{
	GameObject* game_object = new GameObject();
	
	// Load Transform
	Utilities::ImportTransform(ai_node, game_object);

	// Load Mesh
	Utilities::ImportMeshes(ai_node, game_object);

	// Load Texture?


	game_object_nodes.push_back(game_object);
}

void Importer::Scenes::Utilities::ImportTransform(const aiNode* ai_node, GameObject* game_object_node)
{
	aiTransform ai_t;																						// Transform structure for Assimp. aiVector3D and aiQuaternion.
	Transform	ma_t;																						// Transform structure for MathGeoLib. float3 and Quat.

	ai_node->mTransformation.Decompose(ai_t.scale, ai_t.rotation, ai_t.position);							// --- Getting the Transform stored in the node.

	ma_t.position	= { ai_t.position.x, ai_t.position.y, ai_t.position.z };								// 
	ma_t.rotation	= { ai_t.rotation.x, ai_t.rotation.y, ai_t.rotation.z, ai_t.rotation.w };				// 
	ma_t.scale		= { ai_t.scale.x, ai_t.scale.y, ai_t.scale.z };											// ---------------------------------------------

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
		}
	}

	game_object_node->GetTransformComponent()->ImportTransform(ma_t.position, ma_t.rotation, ma_t.scale);	// Importing the final Transform into the game object's transform component.
}

void Importer::Scenes::Utilities::ImportMeshes(const aiNode* ai_node, GameObject* game_object_node)
{

}

void Importer::Scenes::Save(const aiScene* ai_scene, char** buffer)
{

}

void Importer::Scenes::Load(const char* buffer, aiScene* ai_scene)
{

}
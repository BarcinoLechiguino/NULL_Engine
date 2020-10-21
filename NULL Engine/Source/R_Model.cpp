#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#include "R_Mesh.h"
#include "I_Meshes.h"

#include "R_Model.h"

R_Model::R_Model() : Resource()
{

}

R_Model::~R_Model()
{

}

void R_Model::Draw()
{
	for (int i = 0; i < meshes.size(); ++i)
	{
		meshes[i]->Draw();
	}
}

void R_Model::ProcessScene(const aiScene* scene)
{
	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG("[ERROR] Error loading scene %s", aiGetErrorString());

		return;
	}
	
	ProcessNode(scene->mRootNode, scene);
}

void R_Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (uint i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		ProcessMesh(mesh, scene);
	}

	for (uint i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void R_Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	R_Mesh* r_mesh = new R_Mesh();

	Importer::Meshes::Import(mesh, r_mesh);

	meshes.push_back(r_mesh);
}

void R_Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string type_name)
{

}
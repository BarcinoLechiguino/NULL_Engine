#include "Assimp.h"

#include "R_Mesh.h"
#include "I_Meshes.h"

#include "R_Model.h"

R_Model::R_Model(std::string full_path, vec4 colour) : Resource(), full_path(full_path), colour(colour)
{

}

R_Model::~R_Model()
{

}

void R_Model::Draw()
{
	for (uint i = 0; i < meshes.size(); ++i)
	{
		meshes[i]->Draw(colour);
	}
}

void R_Model::DrawNormals()
{
	for (uint i = 0; i < meshes.size(); ++i)
	{
		meshes[i]->DrawNormals();
	}
}

void R_Model::DrawTexCoords()
{
	for (uint i = 0; i < meshes.size(); ++i)
	{
		meshes[i]->DrawTexCoords();
	}
}

void R_Model::SetMaterialColour(vec4 colour)
{
	this->colour = colour;
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

	Importer::Meshes::Import(scene, mesh, r_mesh);

	meshes.push_back(r_mesh);
}

void R_Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, R_Mesh* r_mesh)
{
	for (uint i = 0; i < material->GetTextureCount(type); ++i)
	{
		aiString path;
		material->GetTexture(type, i, &path);

		bool skip = false;

		for (uint j = 0; j < loaded_textures.size(); ++j)
		{
			if (strcmp(path.C_Str(), loaded_textures[j]->path.c_str()) == 0)
			{
				r_mesh->textures.push_back(*loaded_textures[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Texture texture;
			texture.id = 0 /*TextureFromFile(path.C_Str(), directory)*/;
			texture.type = (TEXTURE_TYPE)type;
			texture.path = path.C_Str();

			r_mesh->textures.push_back(texture);
			loaded_textures.push_back(&texture);
		}
	}
}
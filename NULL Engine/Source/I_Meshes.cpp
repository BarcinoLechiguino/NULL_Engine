// ----------------------------------------------------
// I_Meshes.cpp --- Importing .FBX files into meshes.
// Interface class between the engine and Assimp.
// ----------------------------------------------------

#include "Assimp.h"
#include "MathStructures.h"
#include "Log.h"

#include "Application.h"
#include "M_FileSystem.h"
#include "R_Mesh.h"

#include "I_Meshes.h"

#include "MemoryManager.h"

#pragma comment (lib, "Source/Dependencies/Assimp/libx86/assimp.lib")

using namespace Importer::Meshes;																	// Not a good thing to do but it will be employed sparsely and only inside this .cpp

uint64 Importer::Meshes::Save(const R_Mesh* mesh, char** buffer)
{
	uint64 buffer_size = 0;

	aiNode* node = nullptr;

	return buffer_size;
}

void Importer::Meshes::Load(const char* buffer, R_Mesh* mesh)
{

}

void Importer::Meshes::Import(const char* path, std::vector<R_Mesh*>& meshes)
{
	//const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	char* buffer = nullptr;
	uint file_size = App->file_system->Load(path, &buffer);

	if (file_size > 0)
	{
		const aiScene* scene = aiImportFileFromMemory(buffer, file_size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);

		if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LOG("[ERROR] Error loading scene %s", aiGetErrorString());

			return;
		}

		Utilities::ProcessNode(scene, scene->mRootNode, meshes);
	}
}

void Utilities::ProcessNode(const aiScene* scene, aiNode* node, std::vector<R_Mesh*>& meshes)		// Shortened with the use of used namespaces, otherwise it would be a bad idea.
{
	for (uint i = 0; i < node->mNumMeshes; ++i)														// The loop is run for as many meshes that the node has registered it has.
	{
		aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[i]];											// Gets the mesh specifiec by the mesh index stored in the node's mMeshes array.

		if (ai_mesh != nullptr && ai_mesh->HasFaces())												// Checks that the aiMesh is valid.
		{
			R_Mesh* r_mesh			= new R_Mesh();													// Generates a new R_Mesh.
			r_mesh->base_transform	= new Transform();

			Utilities::GenerateMesh(scene, ai_mesh, r_mesh);										// Sets the given r_mesh with the data stored in ai_mesh.
			Utilities::GenerateTransform(scene, node, r_mesh->base_transform);

			if (r_mesh != nullptr)																	// Checks that the R_Mesh* is valid/stores data.
			{
				meshes.push_back(r_mesh);															// Adds the R_Mesh* to the given meshes vector.
			}
			else
			{
				delete r_mesh;
				r_mesh = nullptr;

				LOG("[ERROR] Could not generate a mesh during Import: R_Mesh* was nullptr!");
			}
		}
		else
		{
			LOG("[ERROR] Could not generate a mesh during Import: aiMesh* was nullptr and/or did not have any faces!");
		}
	}

	for (uint i = 0; i < node->mNumChildren; ++i)
	{
		Utilities::ProcessNode(scene, node->mChildren[i], meshes);
	}
}

void Importer::Meshes::Utilities::GenerateMesh(const aiScene* ai_scene, const aiMesh* ai_mesh, R_Mesh* r_mesh)
{
	// Allocating the required memory for each vector
	uint vertices_size = ai_mesh->mNumVertices * 3;											// There will be 3 coordinates per vertex, hence the size will be numVertices * 3.
	r_mesh->vertices.resize(vertices_size);													// Allocating in advance the required memory to store all the verts.

	uint normals_size = ai_mesh->mNumVertices * 3;											// There will be 3 normal coordinates per vertex.
	r_mesh->normals.resize(normals_size);

	uint tex_coords_size = ai_mesh->mNumVertices * 2;										// There will be 2 tex coordinates per vertex.
	r_mesh->tex_coords.resize(tex_coords_size);

	uint indices_size = ai_mesh->mNumFaces * 3;												// The size of the indices vector will be equal to the amount of faces times 3 (triangles).
	r_mesh->indices.resize(indices_size);													// Allocating in advance the required memory to store all the indices.

	// Loading the data from the mesh into the corresponding vectors
	Utilities::GetVertices(ai_mesh, r_mesh, vertices_size);									// 
	Utilities::GetNormals(ai_mesh, r_mesh, normals_size);									// 
	Utilities::GetTexCoords(ai_mesh, r_mesh, tex_coords_size);								// 
	Utilities::GetIndices(ai_mesh, r_mesh, indices_size);									// 

	Utilities::GetTexturePaths(ai_scene, ai_mesh, r_mesh);									// Will get the filename associated with ai_mesh and store it inside r_mesh's tex path vector.

	r_mesh->LoadBuffers();
}

void Importer::Meshes::Utilities::GenerateTransform(const aiScene* ai_scene, const aiNode* ai_node, Transform* transform)
{
	aiTransform ai_tfm;

	ai_node->mTransformation.Decompose(ai_tfm.scale, ai_tfm.rotation, ai_tfm.position);								// --- Getting the mesh transform stored in the node.

	transform->position = { ai_tfm.position.x, ai_tfm.position.y, ai_tfm.position.z };								//
	transform->rotation = { ai_tfm.rotation.x, ai_tfm.rotation.y, ai_tfm.rotation.z, ai_tfm.rotation.w };			//
	transform->scale	= { ai_tfm.scale.x, ai_tfm.scale.y, ai_tfm.scale.z };										// --------------------------------------------------

	std::string node_name = ai_node->mName.C_Str();																	// Assimp generates dummy nodes to stack multiple fbx transformations.
	bool found_dummy_node = true;																					// All those transformations will be collapsed to the first non-dummy node.
	while (found_dummy_node)																						
	{
		found_dummy_node = false;

		if (node_name.find("_$AssimpFbx$_") != std::string::npos && ai_node->mNumChildren == 1)						// All dummy models will contain the "_$AssimpFbx$_" str and only one child.
		{
			ai_node = ai_node->mChildren[0];																		// As dummies only have one child, it will be rather easy to select it.
			
			ai_node->mTransformation.Decompose(ai_tfm.scale, ai_tfm.rotation, ai_tfm.position);						// --- Getting the transform in the dummy's child node.

			float3	d_position	= { ai_tfm.position.x, ai_tfm.position.y, ai_tfm.position.z };						//
			Quat	d_rotation	= { ai_tfm.rotation.x, ai_tfm.rotation.y, ai_tfm.rotation.z, ai_tfm.rotation.w };	//
			float3	d_scale		= { ai_tfm.scale.x, ai_tfm.scale.y, ai_tfm.scale.z };								// ----------------------------------------------------

																													// --- Adding the child's transform to the current one.
			transform->position.Add(d_position);																	// position = position + d_position;
			transform->rotation.Mul(d_rotation);																	// rotation = rotation * d_rotation;
			transform->scale.Mul(d_scale);																			// scale	= scale * d_scale;	(s.x * ds.x, s.y * ds.y, s.z * ds.z);

			node_name = ai_node->mName.C_Str();																		// The dummy's child will be set to be processed next.
			found_dummy_node = true;
		}
	}
}

void Importer::Meshes::Utilities::GetVertices(const aiMesh* ai_mesh, R_Mesh* r_mesh, uint size)
{
	if (ai_mesh->HasPositions())
	{
		memcpy(&r_mesh->vertices[0], ai_mesh->mVertices, sizeof(float) * size);	// &r_mesh->vertices[0] gets a pointer to the beginning of the vector. mVertices is a 3D vector.

		LOG("[STATUS] Imported %u position vertices!", size);
	}
	else
	{
		LOG("[ERROR] Imported Mesh has no position vertices!");
	}
}

void Importer::Meshes::Utilities::GetNormals(const aiMesh* ai_mesh, R_Mesh* r_mesh, uint size)
{
	if (ai_mesh->HasNormals())
	{
		memcpy(&r_mesh->normals[0], ai_mesh->mNormals, sizeof(float) * size);

		LOG("[STATUS] Imported %u normals!", size);
	}
	else
	{
		LOG("[ERROR] Imported Mesh has no normals!");
	}
}

void Importer::Meshes::Utilities::GetTexCoords(const aiMesh* ai_mesh, R_Mesh* r_mesh, uint size)
{
	if (ai_mesh->HasTextureCoords(0))
	{
		for (uint i = 0; i < ai_mesh->mNumVertices; ++i)
		{
			r_mesh->tex_coords[i * 2] = ai_mesh->mTextureCoords[0][i].x;
			r_mesh->tex_coords[i * 2 + 1] = ai_mesh->mTextureCoords[0][i].y;
		}

		LOG("[STATUS] Imported %u texture coordinates!", size);
	}
	else
	{
		LOG("[ERROR] Imported Mesh has no tex coords!");
	}
}

void Importer::Meshes::Utilities::GetIndices(const aiMesh* ai_mesh, R_Mesh* r_mesh, uint size)
{
	if (ai_mesh->HasFaces())																	// Double checked if for whatever reason Generate Mesh is called independently.
	{
		uint ind = 0;
		for (uint i = 0; i < ai_mesh->mNumFaces; ++i)
		{
			aiFace face = ai_mesh->mFaces[i];													// Getting the face that is currently being iterated.

			if (face.mNumIndices == 3)
			{
				//memcpy(&r_mesh->indices[i * 3], face.mIndices, sizeof(uint) * 3);

				for (uint j = 0; j < face.mNumIndices; ++j)
				{
					r_mesh->indices[ind] = face.mIndices[j];
					++ind;
				}
			}
			else
			{
				LOG("[WARNING] Geometry face %u with != 3 indices!", i);
			}
		}

		LOG("[STATUS] Imported %u mesh indices!", size);
	}
}

void Importer::Meshes::Utilities::GetTexturePaths(const aiScene* ai_scene, const aiMesh* ai_mesh, R_Mesh* r_mesh)
{
	if (ai_mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = ai_scene->mMaterials[ai_mesh->mMaterialIndex];
		uint num_textures = material->GetTextureCount(aiTextureType_DIFFUSE);

		for (uint i = 0; i < num_textures; ++i)
		{
			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, i, &path);

			std::string file_path = path.C_Str();
			file_path = App->file_system->NormalizePath(file_path.c_str());

			uint file_start = file_path.find_last_of("/");

			if (file_start == file_path.npos)													// If no "/" could be found.
			{
				r_mesh->tex_paths.push_back(file_path.c_str());
			}
			else
			{
				uint file_end	= file_path.size();
				file_path		= file_path.substr(file_start, file_end);

				r_mesh->tex_paths.push_back(file_path.c_str());
			}


			//r_mesh->tex_paths.push_back(path.C_Str());

			LOG("[STATUS] Mesh texture file name %u is %s", i, path.C_Str());
		}
	}
}
// ----------------------------------------------------
// I_Meshes.cpp --- Importing .FBX files into meshes.
// Interface class between the engine and Assimp.
// ----------------------------------------------------

#include "Assimp.h"

#include "R_Mesh.h"

#include "I_Meshes.h"

#pragma comment (lib, "Source/Dependencies/Assimp/libx86/assimp.lib")

R_Mesh* Importer::Meshes::Create()
{
	return new R_Mesh();
}

void Importer::Meshes::Import(const aiMesh* ai_mesh, R_Mesh* r_mesh)
{	
	// Allocating the required memory for each vector
	uint size = ai_mesh->mNumVertices;
	r_mesh->vertices.resize(size);															// Allocating in advance the required memory to store all the vertices.

	size = ai_mesh->mNumFaces * 3;															// The size of the indices vector will be equal to the amount of faces times 3 (triangles).
	r_mesh->indices.resize(size);															// Allocating in advance the required memory to store all the indices.

	// Constructing all the vertices of the mesh
	for (uint i = 0; i < ai_mesh->mNumVertices; ++i)
	{
		Vertex vertex;

		if (ai_mesh->HasPositions())
		{
			vertex.position.x = ai_mesh->mVertices[i].x;
			vertex.position.y = ai_mesh->mVertices[i].y;
			vertex.position.z = ai_mesh->mVertices[i].z;
		}
		else
		{
			LOG("[WARNING] Warning! Vertex %d has no position vector!", i);
		}

		if (ai_mesh->HasNormals())
		{
			vertex.normal.x = ai_mesh->mNormals[i].x;
			vertex.normal.y = ai_mesh->mNormals[i].y;
			vertex.normal.z = ai_mesh->mNormals[i].z;
		}
		else
		{
			LOG("[WARNING] Warning! Vertex %d has no position vector!", i);
		}

		if (ai_mesh->HasTextureCoords(0))
		{
			vertex.tex_coords.x = ai_mesh->mTextureCoords[0][i].x;
			vertex.tex_coords.y = ai_mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.tex_coords = vec2(0.0f, 0.0f);
		}

		r_mesh->vertices[i] = vertex;
	}

	uint ind = 0;
	for (uint i = 0; i < ai_mesh->mNumFaces; ++i)
	{
		aiFace face = ai_mesh->mFaces[i];													// Getting the face that is currently being iterated.

		for (uint j = 0; j < face.mNumIndices; ++j)
		{
			r_mesh->indices[ind] = face.mIndices[j];
			++ind;
		}
	}

	if (ai_mesh->mMaterialIndex >= 0)
	{

	}

	r_mesh->CreateAABB();
	r_mesh->LoadBuffers();
}

uint64 Importer::Meshes::Save(const R_Mesh* mesh, char** buffer)
{
	uint64 buffer_size = 0;
	


	return buffer_size;
}

void Importer::Meshes::Load(const char* buffer, R_Mesh* mesh)
{

}
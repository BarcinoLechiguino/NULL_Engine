// ----------------------------------------------------
// I_Meshes.cpp --- Importing .FBX files into meshes.
// Interface class between the engine and Assimp.
// ----------------------------------------------------

#include "Assimp.h"

#include "Log.h"
#include "VariableTypedefs.h"

#include "Application.h"
#include "M_FileSystem.h"

#include "R_Mesh.h"

#include "I_Meshes.h"

#include "MemoryManager.h"

#include "T_Timer.h"

#pragma comment (lib, "Source/Dependencies/Assimp/libx86/assimp.lib")

#define HEADER_SIZE 8																				// Amount of items in the Header Data Array. (Save & Load)

using namespace Importer::Meshes;																	// Not a good thing to do but it will be employed sparsely and only inside this .cpp

void Importer::Meshes::Import(const aiMesh* ai_mesh, R_Mesh* r_mesh)
{
	Utilities::ImportMeshData(ai_mesh, r_mesh);

	char* buffer = nullptr;
	uint64 written = Importer::Meshes::Save(r_mesh, &buffer);
	if (written > 0)
	{
		bool success = Importer::Meshes::Load(buffer, r_mesh);
		if (success)
		{
			LOG("[IMPORTER] Successfully loaded %s from the Library directory!", r_mesh->GetAssetsFile());
		}
		else
		{
			LOG("[ERROR] Could not load %s from the Library directory!", r_mesh->GetAssetsFile());
		}
	}
	else
	{
		LOG("[ERROR] Could not save %s in the Library directory!", r_mesh->GetAssetsFile());
	}

	RELEASE_ARRAY(buffer);
}

void Importer::Meshes::Utilities::ImportMeshData(const aiMesh* ai_mesh, R_Mesh* r_mesh)
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
	Utilities::GetVertices(ai_mesh, r_mesh, vertices_size);									// Gets the vertices data stored in the given ai_mesh.
	Utilities::GetNormals(ai_mesh, r_mesh, normals_size);									// Gets the normals data stored in the given ai_mesh
	Utilities::GetTexCoords(ai_mesh, r_mesh, tex_coords_size);								// Gets the tex coords data stored in the given ai_mesh.
	Utilities::GetIndices(ai_mesh, r_mesh, indices_size);									// Gets the indices data stored in the given ai_mesh.

	r_mesh->LoadBuffers();
	r_mesh->SetAABB();
}

void Importer::Meshes::Utilities::GetVertices(const aiMesh* ai_mesh, R_Mesh* r_mesh, uint size)
{
	if (ai_mesh->HasPositions())
	{
		memcpy(&r_mesh->vertices[0], ai_mesh->mVertices, sizeof(float) * size);						// &r_mesh->vertices[0] gets a pointer to the beginning of the vector.

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
			r_mesh->tex_coords[i * 2]		= ai_mesh->mTextureCoords[0][i].x;
			r_mesh->tex_coords[i * 2 + 1]	= ai_mesh->mTextureCoords[0][i].y;
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
	if (ai_mesh->HasFaces())																		// Double checked if for whatever reason Generate Mesh is called independently.
	{
		for (uint i = 0; i < ai_mesh->mNumFaces; ++i)
		{
			aiFace face		= ai_mesh->mFaces[i];													// Getting the face that is currently being iterated.
			uint num_ind	= face.mNumIndices;

			if (num_ind == 3)
			{
				memcpy(&r_mesh->indices[i * num_ind], face.mIndices, sizeof(uint) * num_ind);		// As each face has 3 elements (vertices), the memcpy will be done 3 vertices at a time.
			}
			else
			{
				LOG("[WARNING] Geometry face %u with != 3 indices!", i);
			}
		}

		LOG("[STATUS] Imported %u mesh indices!", size);
	}
	else
	{
		LOG("[ERROR] Imported Mesh has no faces!");
	}
}

uint64 Importer::Meshes::Save(const R_Mesh* r_mesh, char** buffer)
{
	uint64 written = 0;

	uint header_data[HEADER_SIZE] = {
		r_mesh->vertices.size(),																	// 0 --> Num Vertices
		r_mesh->normals.size(), 																	// 1 --> Num Normals
		r_mesh->tex_coords.size(), 																	// 2 --> Num Texture Coordinates
		r_mesh->indices.size(), 																	// 3 --> Num Indices
		r_mesh->VBO, 																				// 4 --> Vertices Buffer Object ID
		r_mesh->NBO, 																				// 5 --> Normals Buffer Object ID
		r_mesh->TBO, 																				// 6 --> Texture Coordinates Buffer Object ID
		r_mesh->IBO 																				// 7 --> Indices Buffer Object ID
		// Draw Normals Bools?
	};

	uint header_data_size = sizeof(header_data) + sizeof(uint);
	uint array_data_size = (header_data[0] + header_data[1] + header_data[2]) * sizeof(float) + header_data[3] * sizeof(uint);
	uint precalculated_data_size = r_mesh->aabb.NumVertices() * sizeof(float) * 3;

	uint size = header_data_size + array_data_size + precalculated_data_size;

	if (size == 0)
	{
		LOG("[WARNING] Mesh %u had no data to Save!", r_mesh->GetUID());
		return 0;
	}

	*buffer				= new char[size];
	char* cursor		= *buffer;
	uint bytes			= 0;

	// --- HEADER DATA ---
	bytes = sizeof(header_data);
	memcpy_s(cursor, size, header_data, bytes);
	cursor += bytes;

	// --- VERTEX ARRAY DATA ---
	bytes = r_mesh->vertices.size() * sizeof(float);
	memcpy_s(cursor, size, &r_mesh->vertices[0], bytes);
	cursor += bytes;

	bytes = r_mesh->normals.size() * sizeof(float);
	memcpy_s(cursor, size, &r_mesh->normals[0], bytes);
	cursor += bytes;

	bytes = r_mesh->tex_coords.size() * sizeof(float);
	memcpy_s(cursor, size, &r_mesh->tex_coords[0], bytes);
	cursor += bytes;

	bytes = r_mesh->indices.size() * sizeof(uint);
	memcpy_s(cursor, size, &r_mesh->indices[0], bytes);
	cursor += bytes;

	// --- PRECALCULATED DATA ---
	math::vec* aabb_corners = new math::vec[8];
	r_mesh->aabb.GetCornerPoints(aabb_corners);

	bytes = r_mesh->aabb.NumVertices() * sizeof(float) * 3;
	memcpy_s(cursor, size, aabb_corners, bytes);
	cursor += bytes;

	delete[] aabb_corners;

	// --- SAVING THE BUFFER ---
	std::string path = std::string(MESHES_PATH) + std::to_string(r_mesh->GetUID()) + std::string(MESHES_EXTENSION);

	written = App->file_system->Save(path.c_str(), *buffer, size);
	
	if (written > 0)
	{	
		LOG("[IMPORTER] Meshes: Successfully Saved %s into the Library!", r_mesh->GetAssetsFile());
	}
	else
	{
		LOG("[ERROR] Meshes Importer: Could not Save %s into the Library!", r_mesh->GetAssetsFile());
	}

	path.clear();

	return written;
}

bool Importer::Meshes::Load(const char* buffer, R_Mesh* r_mesh)
{
	bool ret = true;

	if (buffer == nullptr || r_mesh == nullptr)
	{
		LOG("[ERROR] Meshes Importer: Could not load Mesh Resource from Library, buffer and/or r_mesh was nullptr!");
		return false;
	}

	/*std::string path	= std::string(MESHES_PATH) + std::to_string(r_mesh->GetID()) + std::string(MESH_EXTENSION);
	char* read_buffer	= nullptr;
	uint read			= App->file_system->Load(path.c_str(), &read_buffer);
	if (read == 0)
	{
		// --- LOAD MESH DATA ---
		LOG("[BRUH] Bruh");
		return false;
	}*/
	
	char* cursor	= (char*)buffer;
	uint bytes		= 0;

	// --- HEADER DATA ---
	uint header_data[HEADER_SIZE];

	bytes = sizeof(header_data);
	memcpy_s(header_data, bytes, cursor, bytes);
	cursor += bytes;

	// --- VERTEX ARRAY DATA ---
	r_mesh->vertices.resize(header_data[0]);
	bytes = header_data[0] * sizeof(float);
	memcpy_s(&r_mesh->vertices[0], bytes, cursor, bytes);
	cursor += bytes;

	r_mesh->normals.resize(header_data[1]);
	bytes = header_data[1] * sizeof(float);
	memcpy_s(&r_mesh->normals[0], bytes, cursor, bytes);
	cursor += bytes;

	r_mesh->tex_coords.resize(header_data[2]);
	bytes = header_data[2] * sizeof(float);
	memcpy_s(&r_mesh->tex_coords[0], bytes, cursor, bytes);
	cursor += bytes;

	r_mesh->indices.resize(header_data[3]);
	bytes = header_data[3] * sizeof(uint);
	memcpy_s(&r_mesh->indices[0], bytes, cursor, bytes);
	cursor += bytes;

	r_mesh->VBO = header_data[4];
	r_mesh->NBO = header_data[5];
	r_mesh->TBO = header_data[6];
	r_mesh->IBO = header_data[7];

	// --- PRECALCULATED DATA ---
	float3 aabb_corners[8];

	bytes = sizeof(aabb_corners);
	memcpy_s(aabb_corners, bytes, cursor, bytes);
	cursor += bytes;

	r_mesh->aabb = AABB(aabb_corners[0], aabb_corners[7]);

	// --- FREEING THE BUFFERS ---


	return ret;
}
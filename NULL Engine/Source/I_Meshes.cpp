// ----------------------------------------------------
// I_Meshes.cpp --- Importing .FBX files into meshes.
// Interface class between the engine and Assimp.
// ----------------------------------------------------

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#include "R_Mesh.h"

#include "I_Meshes.h"

#pragma comment (lib, "Source/Dependencies/Assimp/libx86/assimp.lib")

R_Mesh* Importer::Meshes::Create()
{
	return new R_Mesh();
}

void Importer::Meshes::Import(const aiMesh* imported_mesh, R_Mesh* resource_mesh)
{

}

uint64 Importer::Meshes::Save(const R_Mesh* mesh, char** buffer)
{
	uint64 buffer_size = 0;
	


	return buffer_size;
}

void Importer::Meshes::Load(const char* buffer, R_Mesh* mesh)
{

}
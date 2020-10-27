// ----------------------------------------------------
// I_Materials.cpp --- Importing textures from files.
// Interface class between engine and Assimp + DevIL.
// ----------------------------------------------------

#include "Assimp.h"
#include "DevIL.h"

#include "Globals.h"
#include "R_Material.h"

#include "I_Materials.h"

#pragma comment (lib, "Source/Dependencies/DevIL/libx86/DevIL.lib")
#pragma comment (lib, "Source/Dependencies/DevIL/libx86/ILU.lib")
#pragma comment (lib, "Source/Dependencies/DevIL/libx86/ILUT.lib")

R_Material* Importer::Materials::Create()
{	
	return new R_Material();
}

// Importing, saving and loading aiMaterials with Assimp.
void Importer::Materials::Assimp::Import(const aiMaterial* ai_material, R_Material* r_material)
{

}

uint64 Importer::Materials::Assimp::Save(const R_Material* r_material, char** buffer)
{
	uint64 size = 0;
	
	return size;
}

void Importer::Materials::Assimp::Load(const char* buffer, R_Material* r_material)
{

}

// Importing, saving and loading materials with DevIL.
void Importer::Materials::DevIL::Init()
{
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION || ilGetInteger(ILU_VERSION_NUM) < ILU_VERSION  || ilGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION)
	{
		LOG("[ERROR] DevIL version does not match with DevIL lib versions!");
	}
}

void Importer::Materials::DevIL::Import(const char* buffer, uint size, R_Material* r_material)
{

}

uint64 Importer::Materials::DevIL::Save(const R_Material* r_material, char** buffer)
{
	uint64 size = 0;

	return size;
}

void Importer::Materials::DevIL::Load(const char* buffer, uint size, R_Material* r_material)
{

}
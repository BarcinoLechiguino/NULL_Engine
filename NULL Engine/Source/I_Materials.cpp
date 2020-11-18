// ----------------------------------------------------
// I_Materials.cpp --- Importing materials from files.
// Interface class between engine and Assimp.
// ----------------------------------------------------

#include "OpenGL.h"
#include "Assimp.h"
#include "DevIL.h"
#include "Log.h"

#include "Application.h"
#include "M_FileSystem.h"
#include "R_Material.h"
#include "R_Texture.h"

#include "I_Materials.h"

#include "MemoryManager.h"

#pragma comment (lib, "Source/Dependencies/DevIL/libx86/DevIL.lib")
#pragma comment (lib, "Source/Dependencies/DevIL/libx86/ILU.lib")
#pragma comment (lib, "Source/Dependencies/DevIL/libx86/ILUT.lib")

using namespace Importer::Materials;																	// Not a good thing to do but it will be employed sparsely and only inside this .cpp

// Importing, saving and loading aiMaterials with Assimp.
void Importer::Materials::Import(const char* path, const aiMaterial* ai_material, R_Material* r_material, R_Texture* r_texture)		// R_Texture* TMP until the Resource Manager is in place.
{

}

uint64 Importer::Materials::Save(const R_Material* r_material, char** buffer)
{
	uint64 size = 0;
	
	return size;
}

void Importer::Materials::Load(const char* buffer, R_Material* r_material)
{

}
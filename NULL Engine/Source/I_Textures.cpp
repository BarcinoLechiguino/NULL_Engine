// ----------------------------------------------------
// I_Textures.cpp --- Importing textures from files.
// Interface class between engine and DevIL.
// ----------------------------------------------------

#include "OpenGL.h"
#include "DevIL.h"
#include "Log.h"

#include "Application.h"
#include "M_FileSystem.h"

#include "R_Texture.h"

#include "I_Textures.h"

#include "MemoryManager.h"

#pragma comment (lib, "Source/Dependencies/DevIL/libx86/DevIL.lib")
#pragma comment (lib, "Source/Dependencies/DevIL/libx86/ILU.lib")
#pragma comment (lib, "Source/Dependencies/DevIL/libx86/ILUT.lib")

using namespace Importer::Textures;																		// Not a good thing to do but it will be employed sparsely and only inside this .cpp

void Importer::Textures::Utilities::Init()
{
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION || ilGetInteger(ILU_VERSION_NUM) < ILU_VERSION || ilGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION)
	{
		LOG("[ERROR] DevIL Version does not match with lib version.");
	}

	LOG("[STATUS] Initializing DevIL");
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

void Importer::Textures::Utilities::CleanUp()
{
	LOG("[IMPORTER] Freeing all the buffers related to textures.");

	LOG("[STATUS] Shutting down DevIL");
	ilShutDown();
}

void Importer::Textures::Import(const char* path, R_Texture* r_texture)
{

}

uint64 Importer::Textures::Save(const R_Texture* r_texture, char** buffer)
{

}

void Importer::Textures::Load(const char* buffer, R_Texture* r_texture)
{

}
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

bool Importer::Textures::Import(const char* path, R_Texture* r_texture)
{
	bool ret = true;

	LOG("[IMPORTER] Loading %s texture.", path);

	// Create a tex buffer and a tex id.

	// Check that path is not nullptr

	// Use ilGenImages() and ilBindImage() to generate and bind the tex buffer.

	// Load the data in the path using the file system.

	// If the file size is not 0

	// Identify the type of the texture and leave it in uknown if the type does not match with the extension.

	// Use ilLoadL() to import a texture from a buffer.

	// If ilLoadL() is successful, load the texture to a OpenGL buffer

	// Convert the image to the type you want. (IL_RGBA, IL_UNSIGNED_BYTE)

	// Create the texture using CreateTexture();

	// if the tex_id returned by CreateTexture is not 0

	// Assign the data to the tex_data variable in R_Texture*

	// Else LOG an error with the path.


	return ret;
}

uint Importer::Textures::Utilities::CreateTexture(const void* data, uint width, uint height, int internal_format, uint format, uint target, int filter_type, int filling_type)
{
	uint texture_id = 0; 

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, (GLuint*)&texture_id);
	glBindTexture(target, texture_id);

	glTexParameteri(target, GL_TEXTURE_WRAP_S, filling_type);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, filling_type);

	if (filter_type == GL_NEAREST)
	{
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	else if (filter_type == GL_LINEAR)
	{
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		LOG("[ERROR] Invalid filter type! Supported filters: GL_LINEAR and GL_NEAREST.");
	}

	glTexImage2D(target, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(target);

	glBindTexture(target, 0);

	if (texture_id != 0)
	{
		LOG("[STATUS] Texture Successfully loaded! Id: %u, Size: %u x %u", texture_id, width, height);
	}

	return texture_id;
}

uint64 Importer::Textures::Save(const R_Texture* r_texture, char** buffer)
{
	uint64 size = 0;
	
	return size;
}

void Importer::Textures::Load(const char* buffer, R_Texture* r_texture)
{

}
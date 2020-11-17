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

	if (r_texture == nullptr)																					// Safety check to avoid importing textures for non-existent Texture Resources.
	{
		LOG("[ERROR] Texture Import: R_Texture* was nullptr!");
		return false;
	}
	
	LOG("[IMPORTER] Loading %s texture.", path);

	uint tex_buffer;
	uint tex_id;

	if (path != nullptr)
	{
		ilGenImages(1, (ILuint*)&tex_buffer);
		ilBindImage(tex_buffer);

		char* tex_data;
		uint file_size = App->file_system->Load(path, &tex_data);

		if (file_size > 0)
		{
			ILenum type = Utilities::GetTextureType(path);														// ILenum is a typedef for unsigned int, which makes it equivalent to our uint.

			bool success = ilLoadL(type, (const void*)tex_data, file_size);
			if (success)
			{
				tex_id = ilutGLBindTexImage();

				success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
				if (success)
				{
					tex_id = Utilities::CreateTexture(ilGetData(), ilGetInteger(IL_IMAGE_WIDTH),
																	ilGetInteger(IL_IMAGE_HEIGHT),
																	ilGetInteger(IL_IMAGE_FORMAT),
																	ilGetInteger(IL_IMAGE_FORMAT),
																	GL_TEXTURE_2D, GL_LINEAR, GL_REPEAT);

					if (tex_id != 0)
					{
						//r_texture->
					}
					else
					{
						LOG("[ERROR] Could not get texture ID! Path: %s", path);
					}
				}
			}
			else
			{
				ILenum error = ilGetError();
				LOG("[ERROR] ilLoadL() Error: %s", iluErrorString(error));
			}
		}
		else
		{
			LOG("[ERROR] File System could not load the texture: File size was 0!");
		}
	}
	else
	{
		LOG("[ERROR] Texture could not be imported: Path was nullptr!");
	}

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

	if (filter_type == GL_NEAREST)																				// Nearest filtering gets the color of the nearest neighbour pixel.
	{
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	else if (filter_type == GL_LINEAR)																			// Linear filtering interpolates the color of the neighbour pixels.
	{
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))												// In case Anisotropic filtering is available, it will be used.
		{
			GLfloat max_anisotropy;

			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy);
			glTexParameteri(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy);
		}
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

uint Importer::Textures::Utilities::GetTextureType(const char* path)
{
	uint type = IL_TYPE_UNKNOWN;

	std::string extension = App->file_system->GetFileExtension(path);

	if (extension == "png" || extension == "PNG")
	{
		type = IL_PNG;
	}
	else if (extension == "dds" || extension == "DDS")
	{
		type = IL_DDS;
	}
	else if (extension == "tga" || extension == "TGA")
	{
		type = IL_TGA;
	}
	else
	{
		LOG("[WARNING] Texture %s: Could not get texture type from extension!", path);
	}

	return type;
}

uint64 Importer::Textures::Save(const R_Texture* r_texture, char** buffer)
{
	uint64 size = 0;
	
	return size;
}

void Importer::Textures::Load(const char* buffer, R_Texture* r_texture)
{

}
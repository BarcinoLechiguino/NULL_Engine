// ----------------------------------------------------
// I_Textures.cpp --- Importing textures from files.
// Interface class between engine and DevIL.
// ----------------------------------------------------

#include "OpenGL.h"
#include "DevIL.h"
#include "Log.h"

#include "Random.h"

#include "Application.h"
#include "M_FileSystem.h"

#include "R_Texture.h"

#include "I_Textures.h"

#include "MemoryManager.h"

#pragma comment (lib, "Source/Dependencies/DevIL/libx86/DevIL.lib")
#pragma comment (lib, "Source/Dependencies/DevIL/libx86/ILU.lib")
#pragma comment (lib, "Source/Dependencies/DevIL/libx86/ILUT.lib")

using namespace Importer::Textures;																				// Not a good thing to do but it will be used sparsely and only inside this .cpp

void Importer::Textures::Init()
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

void Importer::Textures::CleanUp()
{
	LOG("[STATUS] Shutting down DevIL");
	ilShutDown();
}

uint Importer::Textures::Import(const char* path, R_Texture* r_texture)											// FIX: FLIP TEXTURE
{
	uint tex_id = 0;

	LOG("[IMPORTER] Loading %s texture.", path);

	if (path == nullptr || r_texture == nullptr)
	{
		LOG("[ERROR] Texture could not be imported: Path and/or R_Texture* was nullptr!");
		return 0;
	}

	char* tex_data = nullptr;																				// Buffer where the binary data of the texture to import will be stored.
	uint file_size = App->file_system->Load(path, &tex_data);												// Generating a buffer with the data of the texture in the given path.

	if (tex_data != nullptr && file_size > 0)																// If the buffer is not still nullptr and the file could be loaded.
	{
		ILuint il_image;	//This = 0																		// Will be used to generate, bind and delete the buffers created by DevIL.
		ilGenImages(1, &il_image);																			// DevIL's buffers work pretty much the same way as OpenGL's do.
		ilBindImage(il_image);																				// The first step is to generate a buffer and then bind it.

		bool success = ilLoadL(IL_TYPE_UNKNOWN, (const void*)tex_data, file_size);							// ilLoadL() loads a texture from some buffer data. size == 0 = no bounds check.
		if (success)																						// --- When type is IL_TYPE_UNKNOWN, DevIL will try to find the type on it's own.
		{
			success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);											// Will convert the image to the given format and type. ERROR if out of memory. 
			if (success)
			{
				uint width		= (uint)ilGetInteger(IL_IMAGE_WIDTH);										// --------------------- Getting the imported texture's data.
				uint height		= (uint)ilGetInteger(IL_IMAGE_HEIGHT);										// 
				uint format		= (uint)ilGetInteger(IL_IMAGE_FORMAT);										// Internal format will be forced to be the same as format.
				uint target		= (uint)GL_TEXTURE_2D;														// 
				int wrapping	= (int)GL_REPEAT;															// 
				int filter		= (int)GL_LINEAR;															// ----------------------------------------------------------

				tex_id = Utilities::CreateTexture(ilGetData(), width, height, target, wrapping, filter, format, format);	// Creates an OpenGL texture with the given data and parameters.

				if (tex_id != 0)																							// If tex_id == 0, then it means the tex. could not be created.
				{
					std::string file = App->file_system->GetFileAndExtension(path);

					r_texture->SetTextureData(path, file.c_str(), tex_id, width, height, 0, 0, 0, (TEXTURE_FORMAT)format);

					file.clear();

					char* buffer = nullptr;
					
					uint64 buffer_size = Importer::Textures::Save(r_texture, &buffer);
					
					if (buffer_size > 0)
					{
						Importer::Textures::Load(buffer, buffer_size, r_texture);
					}
					
					RELEASE_ARRAY(buffer);
				}
				else
				{
					LOG("[ERROR] Could not get texture ID! Path: %s", path);
					tex_id = 0;
				}
			}
			else
			{
				LOG("[ERROR] ilConvertImage() Error: %s", iluErrorString(ilGetError()));
				tex_id = 0;
			}
		}
		else
		{
			LOG("[ERROR] ilLoadL() Error: %s", iluErrorString(ilGetError()));
			tex_id = 0;
		}

		ilDeleteImages(1, &il_image);
	}
	else
	{
		LOG("[ERROR] File System could not load the texture: File size was 0!");
		tex_id = 0;
	}

	RELEASE_ARRAY(tex_data);

	return tex_id;
}

uint64 Importer::Textures::Save(const R_Texture* r_texture, char** buffer)
{
	uint64 written = 0;

	ilEnable(IL_FILE_OVERWRITE);

	ILuint		size;
	ILubyte* data;

	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);																		// Choosing a specific DXT compression.

	size = ilSaveL(IL_DDS, nullptr, 0);																			// Getting the size of the data buffer.
	if (size > 0)
	{
		data = new ILubyte[size];																				// Allocating the required memory to the data buffer.

		if (ilSaveL(IL_DDS, data, size) > 0)																	// ilSaveL() will save the current image as the specified type.
		{
			*buffer = (char*)data;

			std::string dir_path = TEXTURES_PATH;
			std::string file = std::to_string(Random::LCG::GetRandomUint());
			//std::string file		= std::to_string(Random::PCG::GetRandomUint());

			std::string full_path = dir_path + file;

			written = App->file_system->Save(full_path.c_str(), *buffer, size, false);
			if (written > 0)
			{
				LOG("[IMPORTER] Successfully saved %s in %s", file.c_str(), dir_path.c_str());
			}
			else
			{
				*buffer = nullptr;
				LOG("[ERROR] Could not save %s!", file.c_str());
			}

			dir_path.clear();
			file.clear();
			full_path.clear();
		}
		else
		{
			LOG("[ERROR] ilSaveL() Error: %s", iluErrorString(ilGetError()));
		}
	}
	else
	{
		LOG("[ERROR] ilSaveL() Error: %s", iluErrorString(ilGetError()));
	}

	return written;
}

void Importer::Textures::Load(const char* buffer, const uint size, R_Texture* r_texture)
{
	ilEnable(IL_FILE_OVERWRITE);

	ILuint il_image;
	ilGenImages(1, &il_image);
	ilBindImage(il_image);

	bool success = ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size);
	if (success)
	{
		ilutGLBindTexImage();

		uint width		= (uint)ilGetInteger(IL_IMAGE_WIDTH);													// --------------------- Getting the imported texture's data.
		uint height		= (uint)ilGetInteger(IL_IMAGE_HEIGHT);													// 
		uint format		= (uint)ilGetInteger(IL_IMAGE_FORMAT);													// Internal format will be forced to be the same as format.
		uint target		= (uint)GL_TEXTURE_2D;																	// 
		int wrapping	= (int)GL_REPEAT;																		// 
		int filter		= (int)GL_LINEAR;																		// ----------------------------------------------------------

		r_texture->tex_data.id = Utilities::CreateTexture(ilGetData(), width, height, target, wrapping, filter, format, format);
	}
	else
	{
		LOG("[ERROR] Could not Load() %s from Library!", r_texture->tex_data.file.c_str());
	}

	ilDeleteImages(1, &il_image);
}

uint Importer::Textures::Utilities::CreateTexture(const void* data, uint width, uint height, uint target, int wrapping, int filter, int internal_format, uint format)
{
	uint texture_id = 0; 

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, (GLuint*)&texture_id);
	glBindTexture(target, texture_id);

	glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapping);

	if (filter == GL_NEAREST)																					// Nearest filtering gets the color of the nearest neighbour pixel.
	{
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	else if (filter == GL_LINEAR)																				// Linear filtering interpolates the color of the neighbour pixels.
	{
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))												// In case Anisotropic filtering is available, it will be used.
		{
			GLfloat max_anisotropy;

			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy);
			glTexParameteri(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLint)max_anisotropy);
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
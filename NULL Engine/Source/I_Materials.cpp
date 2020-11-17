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

#include "I_Materials.h"

#include "MemoryManager.h"

#pragma comment (lib, "Source/Dependencies/DevIL/libx86/DevIL.lib")
#pragma comment (lib, "Source/Dependencies/DevIL/libx86/ILU.lib")
#pragma comment (lib, "Source/Dependencies/DevIL/libx86/ILUT.lib")

using namespace Importer::Materials;																	// Not a good thing to do but it will be employed sparsely and only inside this .cpp

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
		LOG("[ERROR] DevIL version does not match with lib version!");
	}

	LOG("[STATUS] Initializing DevIL");
	ilInit();																	// Initializing IL.			// DevIL is automatically initialized when it is loaded by the application.
	iluInit();																	// Initializing ILU.		// However it is recommended to avoid any porting issues.
	ilutInit();																	// Initializing ILUT.
	ilutRenderer(ILUT_OPENGL);
}

void Importer::Materials::DevIL::CleanUp()
{
	LOG("[STATUS] Freeing all the buffers related with textures.");



	LOG("[STATUS] Shutting down DevIL.");
	ilShutDown();
}

bool Importer::Materials::DevIL::Import(const char* path, R_Material* r_material)
{
	bool ret = true;
	
	LOG("[STATUS] Loading %s", path);
	
	uint	tex_buffer;
	uint	tex_id;

	if (path != nullptr)
	{
		ilGenImages(1, (ILuint*)&tex_buffer);												// DevIL's buffers work pretty much the same way as OpenGL's do.
		ilBindImage(tex_buffer);															// The first step is to generate a buffer and then bind it.

		char* data = nullptr;
		uint file_size = App->file_system->Load(path, &data);

		if (file_size > 0)
		{
			ILenum type = IL_TYPE_UNKNOWN;														// When type is IL_TYPE_UNKNOWN, it tells DevIL to try to determine the type on its own.
			
			std::string extension = App->file_system->GetFileExtension(path);
			
			if (extension == "png" || extension == "PNG")										// Checking for the .png extension. Also checking caps just in case.
			{
				type = IL_PNG;																	// Changing the type to IL_PNG so it tells DevIL that it is loading a .png file.
			}																					
			else if (extension == "dds" || extension == "DDS")									// Checking for the .dds extension. Also checking caps just in case.
			{
				type = IL_DDS;																	// Changing the type to IL_DDS so it tells DevIL that it is loading a .dds file.
			}
			else if (extension == "tga" || extension == "TGA")
			{																					
				type = IL_TGA;
			}
			
			bool success = ilLoadL(type, (const void*)data, file_size);							// ilLoadL() loads an image from buffer data. If size = 0 then then no bounds-checking is done.
			if (success)
			{
				tex_id = ilutGLBindTexImage();													// Gets the corresponding OpenGL texture for the one that was just loaded. Use OGL from now on.

				success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
				if (success)																	// It is checked that the currently loaded image's pixel data is in RGBA format.
				{
					tex_id = DevIL::CreateTexture(ilGetData(), ilGetInteger(IL_IMAGE_WIDTH),	// Loading a texture into OpenGL buffers. Use data buffer, for now it does not work though.
																ilGetInteger(IL_IMAGE_HEIGHT), 
																ilGetInteger(IL_IMAGE_FORMAT), 
																ilGetInteger(IL_IMAGE_FORMAT),
																GL_TEXTURE_2D, GL_NEAREST, GL_REPEAT);

					if (tex_id != 0)
					{
						r_material->tex_data.path	= path;										// Filling the Texture struct in R_Material* with the data from the loaded texture.
						r_material->tex_data.type	= TEXTURE_TYPE::ALBEDO;						// For now only diffuse textures will be used.
						r_material->tex_data.id		= tex_id;
						r_material->tex_data.width	= ilGetInteger(IL_IMAGE_WIDTH);
						r_material->tex_data.height = ilGetInteger(IL_IMAGE_HEIGHT);
					}
					else
					{
						LOG("[ERROR] Could not get texture Id! Path: %s", path);	
						ret = false;
					}
				}
				else
				{
					ILenum error = ilGetError();
					LOG("[ERROR] Could not convert texture to the correct pixel format! DevIL Error: %s", iluErrorString(error));
					ret = false;
				}
			}
			else
			{
				ILenum error = ilGetError();
				LOG("[ERROR] Could not load texture from data buffer! DevIL Error: %s", iluErrorString(error));
				ret = false;
			}
		}
		else
		{
			LOG("[ERROR] The loaded file size was %u! Path: %s", path);
			ilDeleteImages(1, &tex_buffer);
			ret = false;
		}
	}

	return ret;
}

uint Importer::Materials::DevIL::CreateTexture(const void* data, uint width, uint height, int internal_format, uint format, uint target, int filter_type, int filling_type)
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
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);											// Anisotropy goes here too?
	}
	else
	{
		LOG("[ERROR] Invalid filter type! Supported filters: GL_LINEAR and GL_NEAREST");
	}

	glTexImage2D(target, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data); 

	glGenerateMipmap(target);

	glBindTexture(target, 0);

	if (texture_id != 0)
	{
		LOG("[STATUS] Texture succesfully loaded! Id: %u, Size: %u x %u", texture_id, width, height);
	}

	return texture_id;
}

uint64 Importer::Materials::DevIL::Save(const R_Material* r_material, char** buffer)
{
	uint64 size = 0;

	return size;
}

void Importer::Materials::DevIL::Load(const char* buffer, uint size, R_Material* r_material)
{

}
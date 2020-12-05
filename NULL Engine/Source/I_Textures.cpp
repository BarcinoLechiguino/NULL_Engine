// ----------------------------------------------------
// I_Textures.cpp --- Importing textures from files.
// Interface class between engine and DevIL.
// ----------------------------------------------------

#include "OpenGL.h"
#include "DevIL.h"

#include "VariableTypedefs.h"
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

	char* tex_data = nullptr;
	uint file_size = App->file_system->Load(path, &tex_data);

	if (tex_data != nullptr && file_size > 0)
	{
		bool success = ilLoadL(IL_TYPE_UNKNOWN, (const void*)tex_data, file_size);
		if (success)
		{	
			char* buffer = nullptr;
			uint written = Importer::Textures::Save(r_texture, &buffer);
			
			if (buffer != nullptr && written > 0)
			{
				r_texture->SetAssetsPath(path);
				r_texture->SetAssetsFile(App->file_system->GetFileAndExtension(path).c_str());
				
				//Importer::Textures::Load(buffer, written, r_texture);
				success = Importer::Textures::Load(r_texture);
				if (success)
				{
					tex_id = r_texture->GetTextureID();
					LOG("[IMPORTER] Successfully loaded %s from Library!", r_texture->GetAssetsFile());
				}
				else
				{
					LOG("[IMPORTER] Could not load %s from Library!", r_texture->GetAssetsFile());
				}
			}
			else
			{
				LOG("[ERROR] Could not Save() %s in the Library!", path);
			}

			RELEASE_ARRAY(buffer);
		}
		else
		{
			LOG("[ERROR] Could not load %s from Assets! ilLoadL() Error: %s", path, iluErrorString(ilGetError()));
		}
	}
	else
	{
		LOG("[ERROR] File System could not load %s!", path);
	}

	RELEASE_ARRAY(tex_data);

	return tex_id;
}

uint64 Importer::Textures::Save(const R_Texture* r_texture, char** buffer)
{
	return 0;
}

uint64 Importer::Textures::Save(R_Texture* r_texture, char** buffer)
{
	uint64 written = 0;
	
	std::string dir_path	= TEXTURES_PATH;																	// --- Getting the path to which save the texture.
	std::string file		= std::to_string(r_texture->GetID()) + std::string(TEXTURE_EXTENSION);				// 
	std::string full_path	= dir_path + file;																	// -----------------------------------------------

	ilEnable(IL_FILE_OVERWRITE);																				// Allowing DevIL to overwrite existing files.
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);																		// Choosing a specific DXT compression.

	ILuint size = ilSaveL(IL_DDS, nullptr, 0);																	// Getting the size of the data buffer in bytes. Compressed DDS, Uncomp. TGA.
	if (size > 0)
	{
		LOG("[IMPORTER] Size: %d x %d", ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
		
		ILubyte* data = new ILubyte[size];																		// Allocating the required memory to the data buffer.

		if (ilSaveL(IL_DDS, data, size) > 0)																	// ilSaveL() will save the current image as the specified type.
		{
			LOG("[IMPORTER] Size: %d x %d", ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
			
			*buffer = (char*)data;
			written = App->file_system->Save(full_path.c_str(), *buffer, size, false);							// Saving the texture throught the file system.
			
			if (written > 0)
			{
				r_texture->SetLibraryPath(full_path.c_str());													// Storing the library_path and the library_file name of the texture.
				r_texture->SetLibraryFile(file.c_str());
				
				LOG("[IMPORTER] Successfully saved %s in %s", file.c_str(), dir_path.c_str());
			}
			else
			{
				*buffer = nullptr;
				LOG("[ERROR] Could not save %s!", file.c_str());
			}
		}
		else
		{
			LOG("[ERROR] Could not save the texture! ilSaveL() Error: %s", iluErrorString(ilGetError()));
		}
	}
	else
	{
		LOG("[ERROR] Could not get the size of the texture to save! ilSaveL() Error: %s", iluErrorString(ilGetError()));
	}

	dir_path.clear();
	file.clear();
	full_path.clear();

	return written;
}

bool Importer::Textures::Load(const char* buffer, const uint size, R_Texture* r_texture)
{
	bool ret = true;
	
	ILuint il_image = 0;																				// Will be used to generate, bind and delete the buffers created by DevIL.
	ilGenImages(1, &il_image);																			// DevIL's buffers work pretty much the same way as OpenGL's do.
	ilBindImage(il_image);																				// The first step is to generate a buffer and then bind it.

	bool success = ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size);									// ilLoadL() loads a texture from some buffer data. size == 0 = no bounds check.
	if (success)																						// --- When type is IL_TYPE_UNKNOWN, DevIL will try to find the type on it's own.
	{
		uint color_channels = ilGetInteger(IL_IMAGE_CHANNELS);
		if (color_channels == 3)
		{
			success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);											// ilConvertImage() will convert the image to the given format and type.
		}
		else if (color_channels == 4)
		{
			success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);										// ilConvertImage() will return false if the system cannot store the image with
		}																								// its new format or the operation is invalid (no bound img. or invalid identifier).
		else
		{
			LOG("[WARNING] Texture has less than 3 color channels! Path: %s", r_texture->GetAssetsPath());
		}

		if (success)
		{
			ILinfo il_info;
			iluGetImageInfo(&il_info);

			if (il_info.Origin == IL_ORIGIN_UPPER_LEFT)
			{
				iluFlipImage();

				/*ilEnable(IL_ORIGIN_SET);
				ilOriginFunc(IL_ORIGIN_LOWER_LEFT);*/
			}

			uint width		= il_info.Width;															// --------------------- Getting the imported texture's data.
			uint height		= il_info.Height;															// 
			uint depth		= il_info.Depth;															// 
			uint bpp		= il_info.Bpp;																// 
			uint size		= il_info.SizeOfData;														// 
			uint format		= il_info.Format;															// Internal format will be forced to be the same as format.
			uint target		= (uint)GL_TEXTURE_2D;														// 
			int wrapping	= (int)GL_REPEAT;															// 
			int filter		= (int)GL_LINEAR;															// ----------------------------------------------------------

			uint tex_id = Utilities::CreateTexture(ilGetData(), width, height, target, wrapping, filter, format, format);	// Creates an OpenGL texture with the given data and parameters.

			if (tex_id != 0)																								// If tex_id == 0, then it means the tex. could not be created.
			{
				r_texture->SetTextureData(tex_id, width, height, depth, bpp, size, (TEXTURE_FORMAT)format);
			}
			else
			{
				LOG("[ERROR] Could not get texture ID! Path: %s", r_texture->GetAssetsPath());
				ret = false;
			}
		}
		else
		{
			LOG("[ERROR] ilConvertImage() Error: %s", iluErrorString(ilGetError()));
			ret = false;
		}
	}
	else
	{
		LOG("[ERROR] ilLoadL() Error: %s", iluErrorString(ilGetError()));
		ret = false;
	}

	ilDeleteImages(1, &il_image);

	return ret;
}

bool Importer::Textures::Load(R_Texture* r_texture)
{
	bool ret = true;
	
	char* tex_data = nullptr;																			// Buffer where the binary data of the texture to import will be stored.
	uint file_size = App->file_system->Load(r_texture->GetLibraryPath(), &tex_data);					// Generating a buffer with the data of the texture in the given path.

	if (tex_data == nullptr || file_size == 0)
	{
		LOG("[ERROR] File System could not load tex data! Path: %s", r_texture->GetAssetsPath());
		return false;
	}
	
	ILuint il_image = 0;																				// Will be used to generate, bind and delete the buffers created by DevIL.
	ilGenImages(1, &il_image);																			// DevIL's buffers work pretty much the same way as OpenGL's do.
	ilBindImage(il_image);																				// The first step is to generate a buffer and then bind it.

	bool success = ilLoadL(IL_TYPE_UNKNOWN, (const void*)tex_data, file_size);							// ilLoadL() loads a texture from some buffer data. size == 0 = no bounds check.
	if (success)																						// --- When type is IL_TYPE_UNKNOWN, DevIL will try to find the type on it's own.
	{	
		uint color_channels = ilGetInteger(IL_IMAGE_CHANNELS);
		if (color_channels == 3)
		{
			success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);											// ilConvertImage() will convert the image to the given format and type.
		}
		else if (color_channels == 4)
		{
			success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);										// ilConvertImage() will return false if the system cannot store the image with
		}																								// its new format or the operation is invalid (no bound img. or invalid identifier).
		else
		{
			LOG("[WARNING] Texture has less than 3 color channels! Path: %s", r_texture->GetAssetsPath());
		}

		if (success)
		{
			ILinfo il_info;
			iluGetImageInfo(&il_info);

			if (il_info.Origin == IL_ORIGIN_UPPER_LEFT)
			{
				iluFlipImage();
			}
			
			uint width		= il_info.Width;															// --------------------- Getting the imported texture's data.
			uint height		= il_info.Height;															// 
			uint depth		= il_info.Depth;															// 
			uint bpp		= il_info.Bpp;																// 
			uint size		= il_info.SizeOfData;														// 
			uint format		= il_info.Format;															// Internal format will be forced to be the same as format.
			uint target		= (uint)GL_TEXTURE_2D;														// 
			int wrapping	= (int)GL_REPEAT;															// 
			int filter		= (int)GL_LINEAR;															// ----------------------------------------------------------

			uint tex_id = Utilities::CreateTexture(ilGetData(), width, height, target, wrapping, filter, format, format);	// Creates an OpenGL texture with the given data and parameters.

			if (tex_id != 0)																							// If tex_id == 0, then it means the tex. could not be created.
			{
				r_texture->SetTextureData(tex_id, width, height, depth, bpp, size, (TEXTURE_FORMAT)format);
			}
			else
			{
				LOG("[ERROR] Could not get texture ID! Path: %s", r_texture->GetAssetsPath());
				ret = false;
			}
		}
		else
		{
			LOG("[ERROR] ilConvertImage() Error: %s", iluErrorString(ilGetError()));
			ret = false;
		}
	}
	else
	{
		LOG("[ERROR] ilLoadL() Error: %s", iluErrorString(ilGetError()));
		ret = false;
	}

	ilDeleteImages(1, &il_image);
	
	return ret;
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
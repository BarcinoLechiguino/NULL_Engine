#ifndef __R_TEXTURE_H__
#define __R_TEXTURE_H__

#include "Resource.h"

enum class TEXTURE_FORMAT								// The enum values are set according to the values of DevIL's define values/flags.
{
	UNKNOWN			= 0,								// Default format. It means that DevIL could not find the format of the imported texture.
	COLOUR_INDEX	= 0x1900,							// 0x1900 = IL_COLOUR_INDEX. 
	RGB				= 0x1907,							// 0x1907 = IL_RGB.
	RGBA			= 0x1908,							// 0x1908 = IL_RGBA.
	BGR				= 0x80E0,							// 0x80E0 = IL_BGR.
	BGRA			= 0x80E1,							// 0x80E1 = IL_BGRA.
	LUMINANCE		= 0x1909							// 0x1909 = IL_LUMINANCE.
};

struct Texture
{
	Texture();

	uint			id;									// Id of the texture.
	uint			width;								// Width of the texture in pixels.
	uint			height;								// Height of the texture in pixels.
	std::string		path;								// Path of the file in the directory. Will be used to avoid making duplicates.
	std::string		file;								// File and extension string of the texture. Maybe use path + App->file_system->GetFileAndExtension() instead?
	TEXTURE_FORMAT	format;								// Colour Index, RGB, RGBA... Adapted to fit the formats that DevIL returns.
};

class R_Texture : public Resource
{
public:
	R_Texture();
	~R_Texture();

	bool CleanUp() override;

public:


public:
	void SetTextureData(const char* path, const char* file, TEXTURE_FORMAT format, uint id, uint width, uint height);

public:
	Texture tex_data;

private:

};

#endif // !__R_TEXTURE_H__
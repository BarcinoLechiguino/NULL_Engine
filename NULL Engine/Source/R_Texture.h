#ifndef __R_TEXTURE_H__
#define __R_TEXTURE_H__

#include "Resource.h"

enum class TEXTURE_FORMAT													// The enum values are set according to the values of DevIL's define values/flags.
{
	UNKNOWN			= 0,													// Default format. It means that DevIL could not find the format of the imported texture.
	COLOUR_INDEX	= 0x1900,												// 0x1900 = IL_COLOUR_INDEX. 
	RGB				= 0x1907,												// 0x1907 = IL_RGB.
	RGBA			= 0x1908,												// 0x1908 = IL_RGBA.
	BGR				= 0x80E0,												// 0x80E0 = IL_BGR.
	BGRA			= 0x80E1,												// 0x80E1 = IL_BGRA.
	LUMINANCE		= 0x1909												// 0x1909 = IL_LUMINANCE.
};

struct Texture
{
	Texture();

	uint			id;														// Id of the texture.
	uint			width;													// Width of the texture in pixels.
	uint			height;													// Height of the texture in pixels.
	uint			depth;													// Depth of the texture in pixels.
	uint			bpp;													// Amount of Bytes Per Pixel.
	uint			bytes;													// Size of the texture in bytes.
	TEXTURE_FORMAT	format;													// Colour Index, RGB, RGBA... Adapted to fit the formats that DevIL returns.
	bool			compressed;												// Will be True if the texture comes from a compressed format (DDS, ...).
};

class R_Texture : public Resource
{
public:
	R_Texture();
	~R_Texture();

	bool CleanUp() override;

public:
	const char*		GetAssetsPath			() const;						// 
	const char*		GetAssetsFile			() const;						// 
	const char*		GetLibraryPath			() const;						// 
	const char*		GetLibraryFile			() const;						// 
	
	void			SetAssetsPath			(const char* assets_path);		// 
	void			SetAssetsFile			(const char* assets_file);		// 
	void			SetLibraryPath			(const char* library_path);		// 
	void			SetLibraryFile			(const char* library_file);		// 
	
	Texture			GetTextureData			() const;
	void			SetTextureData			(uint id, uint width, uint height, uint depth, uint bpp, uint bytes, TEXTURE_FORMAT format, bool compressed = true);

	uint			GetTextureID			() const;						// 
	uint			GetTextureWidth			() const;						// 
	uint			GetTextureHeight		() const;						// 
	uint			GetTextureDepth			() const;						// 
	uint			GetTextureBpp			() const;						// 
	uint			GetTextureBytes			() const;						// 
	TEXTURE_FORMAT	GetTextureFormat		() const;						// 
	bool			TextureIsCompressed		() const;						// 

	const char*		GetTextureFormatString	() const;

private:
	std::string		assets_path;											// Path of the file in the Assets directory. Will be used to avoid making duplicates.
	std::string		assets_file;											// File and extension string of the texture. Maybe use path + App->file_system->GetFileAndExtension() instead?

	std::string		library_path;											// Path of the file in the Library directory. Will be used to avoid making duplicates.
	std::string		library_file;											// File and extension string of the texture. Maybe use path + App->file_system->GetFileAndExtension() instead?

	Texture			tex_data;
};

#endif // !__R_TEXTURE_H__
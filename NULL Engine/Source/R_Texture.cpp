#include "OpenGL.h"

#include "R_Texture.h"

R_Texture::R_Texture() : Resource(), 
assets_path(""), 
assets_file(""), 
library_path(""), 
library_file("")
{

}

R_Texture::~R_Texture()
{

}

bool R_Texture::CleanUp()
{
	bool ret = true;

	assets_path.clear();
	assets_file.clear();
	library_path.clear();
	library_file.clear();

	glDeleteTextures(1, (GLuint*)&tex_data.id);

	return ret;
}

// --- R_TEXTURE METHODS ---
const char* R_Texture::GetAssetsPath() const
{
	return assets_path.c_str();
}
const char* R_Texture::GetAssetsFile() const
{
	return assets_file.c_str();
}
const char* R_Texture::GetLibraryPath() const
{
	return library_path.c_str();
}
const char* R_Texture::GetLibraryFile() const
{
	return library_file.c_str();
}

void R_Texture::SetAssetsPath(const char* assets_path)
{
	this->assets_path = assets_path;
}

void R_Texture::SetAssetsFile(const char* assets_file)
{
	this->assets_file = assets_file;
}

void R_Texture::SetLibraryPath(const char* library_path)
{
	this->library_path = library_path;
}

void R_Texture::SetLibraryFile(const char* library_file)
{
	this->library_file = library_file;
}

Texture R_Texture::GetTextureData() const
{
	return tex_data;
}

void R_Texture::SetTextureData(uint id, uint width, uint height, uint depth, uint bpp, uint bytes, TEXTURE_FORMAT format, bool compressed)
{
	tex_data.id				= id;
	tex_data.width			= width;
	tex_data.height			= height;
	tex_data.depth			= depth;
	tex_data.bpp			= bpp;
	tex_data.bytes			= bytes;
	tex_data.format			= format;
	tex_data.compressed		= compressed;
}

uint R_Texture::GetTextureID() const
{
	return tex_data.id;
}

uint R_Texture::GetTextureWidth() const
{
	return tex_data.width;
}

uint R_Texture::GetTextureHeight() const
{
	return tex_data.height;
}

uint R_Texture::GetTextureDepth() const
{
	return tex_data.depth;
}

uint R_Texture::GetTextureBpp() const
{
	return tex_data.bpp;
}

uint R_Texture::GetTextureBytes() const
{
	return tex_data.bytes;
}

TEXTURE_FORMAT R_Texture::GetTextureFormat() const
{
	return tex_data.format;
}

bool R_Texture::TextureIsCompressed() const
{
	return tex_data.compressed;
}

const char* R_Texture::GetTextureFormatString() const
{
	switch (tex_data.format)
	{
	case TEXTURE_FORMAT::UNKNOWN:		{ return "UNKNOWN"; }		break;
	case TEXTURE_FORMAT::COLOUR_INDEX:	{ return "COLOUR_INDEX"; }	break;
	case TEXTURE_FORMAT::RGB:			{ return "RGB"; }			break;
	case TEXTURE_FORMAT::RGBA:			{ return "RGBA"; }			break;
	case TEXTURE_FORMAT::BGR:			{ return "BGR"; }			break;
	case TEXTURE_FORMAT::BGRA:			{ return "BGRA"; }			break;
	case TEXTURE_FORMAT::LUMINANCE:		{ return "LUMINANCE"; }		break;
	}

	return "NONE";
}

// --- TEXTURE STRUCT METHODS ---
Texture::Texture()
{
	id			= 0;
	width		= 0;
	height		= 0;
	depth		= 0;
	bpp			= 0;
	bytes		= 0;
	format		= TEXTURE_FORMAT::UNKNOWN;
	compressed	= true;
}
#include "OpenGL.h"

#include "R_Texture.h"

R_Texture::R_Texture() : Resource()
{

}

R_Texture::~R_Texture()
{

}

bool R_Texture::CleanUp()
{
	bool ret = true;

	tex_data.path.clear();
	tex_data.file.clear();

	glDeleteTextures(1, (GLuint*)&tex_data.id);

	return ret;
}

void R_Texture::SetTextureData(const char* path, const char* file, uint id, uint width, uint height, uint depth, uint bpp, uint bytes, TEXTURE_FORMAT format, bool compressed)
{
	tex_data.path			= path;
	tex_data.file			= file;

	tex_data.id				= id;
	tex_data.width			= width;
	tex_data.height			= height;
	tex_data.depth			= depth;
	tex_data.bpp			= bpp;
	tex_data.bytes			= bytes;
	tex_data.format			= format;
	tex_data.compressed		= compressed;
}

Texture::Texture()
{
	path		= "NONE";
	file		= "NONE";
	
	id			= -1;											// Dirty way to set the id to Non-Valid.
	width		= 0;
	height		= 0;
	depth		= 0;
	bpp			= 0;
	bytes		= 0;
	format		= TEXTURE_FORMAT::UNKNOWN;
	compressed	= true;
}
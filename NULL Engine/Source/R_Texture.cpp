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

void R_Texture::SetTextureData(const char* path, const char* file, TEXTURE_FORMAT format, uint id, uint width, uint height)
{

}

Texture::Texture()
{
	path	= "NONE";
	file	= "NONE";
	format	= TEXTURE_FORMAT::UNKNOWN;
	id		= 0;
	width	= 0;
	height	= 0;
}
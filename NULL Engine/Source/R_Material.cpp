#include "OpenGL.h"

#include "R_Material.h"

R_Material::R_Material() : Resource()
{
	//tex_data = { "NONE", TEXTURE_TYPE::NONE, 0, 0, 0 };

	diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
}

R_Material::~R_Material()
{

}

bool R_Material::CleanUp()
{
	bool ret = true;

	tex_data.path.clear();
	tex_data.file.clear();

	glDeleteTextures(1, (GLuint*)&tex_data.id);													// Free texture buffers.

	return ret;
}

Texture::Texture()
{
	path	= "NONE";
	file	= "NONE";
	type	= TEXTURE_TYPE::NONE;
	id		= 0;
	width	= 0;
	height	= 0;
}
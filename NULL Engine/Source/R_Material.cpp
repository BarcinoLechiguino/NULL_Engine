#include "R_Material.h"

R_Material::R_Material() : Resource()
{

}

R_Material::~R_Material()
{

}

Texture::Texture()
{
	path	= "NONE";
	type	= TEXTURE_TYPE::NONE;
	id		= 0;
	width	= 0;
	height	= 0;
}
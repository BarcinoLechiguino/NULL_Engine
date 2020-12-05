#include "VariableTypedefs.h"

#include "R_Material.h"

R_Material::R_Material() : Resource()
{
	diffuse_color	= { 1.0f, 1.0f, 1.0f, 1.0f };
}

R_Material::~R_Material()
{

}

bool R_Material::CleanUp()
{
	bool ret = true;



	return ret;
}
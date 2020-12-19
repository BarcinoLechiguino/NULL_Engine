#include "VariableTypedefs.h"

#include "R_Material.h"

R_Material::R_Material() : Resource(RESOURCE_TYPE::MATERIAL)
{
	diffuse_color	= { 1.0f, 1.0f, 1.0f, 1.0f };
}

R_Material::~R_Material()
{

}

bool R_Material::CleanUp()
{
	bool ret = true;

	for (uint i = 0; i < materials.size(); ++i)
	{
		materials[i].CleanUp();
	}

	materials.clear();

	return ret;
}

// --- MATERIAL METHODS ---
Material::Material() :
type				(TEXTURE_TYPE::NONE),
texture_uid			(0),
texture_assets_path	("[NONE]")
{

}

Material::Material(TEXTURE_TYPE type, uint texture_uid, std::string texture_assets_path) :
type				(type),
texture_uid			(texture_uid),
texture_assets_path	(texture_assets_path)
{

}

void Material::CleanUp()
{
	texture_assets_path.clear();
}
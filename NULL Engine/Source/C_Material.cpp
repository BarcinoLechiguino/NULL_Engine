#include "GameObject.h"
#include "R_Material.h"

#include "C_Material.h"

C_Material::C_Material(GameObject* owner) : Component(owner, COMPONENT_TYPE::MATERIAL, "Material"),
material(nullptr)
{

}

C_Material::~C_Material()
{

}

bool C_Material::Update()
{
	bool ret = true;

	return ret;
}

bool C_Material::CleanUp()
{
	bool ret = true;

	return ret;
}

// --- C_MATERIAL METHODS ---
R_Material* C_Material::GetMaterial() const
{
	return material;
}

void C_Material::SetMaterial(R_Material* r_material)
{
	material = r_material;
}

std::string C_Material::GetTexturePath() const
{
	if (material != nullptr)
	{
		return material->tex_data.path;
	}

	return "NONE";
}

uint C_Material::GetTextureId() const
{
	if (material != nullptr)
	{
		return material->tex_data.id;
	}

	return 0;
}

void C_Material::GetTextureSize(uint& width, uint& height)
{
	if (material != nullptr)
	{
		width	= material->tex_data.width;
		height	= material->tex_data.height;
	}
}
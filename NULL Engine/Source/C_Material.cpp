#include "Color.h"

#include "GameObject.h"
#include "R_Material.h"

#include "C_Material.h"

C_Material::C_Material(GameObject* owner) : Component(owner, COMPONENT_TYPE::MATERIAL, "Material"),
material(nullptr),
use_default_tex(false)
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

	if (material != nullptr)
	{
		material->CleanUp();
	}

	for (uint i = 0; i < textures.size(); ++i)
	{
		textures[i]->CleanUp();

		delete textures[i];
		textures[i] = nullptr;
	}

	textures.clear();

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

void C_Material::SetMaterialColour(const Color& color)
{
	material->diffuse = color;
}

std::string C_Material::GetTexturePath() const
{
	if (material != nullptr)
	{
		return material->tex_data.path;
	}

	return "NONE";
}

std::string C_Material::GetTextureFile() const
{
	if (material != nullptr)
	{
		std::string file = material->tex_data.path;

		uint file_start = file.find_last_of("/") + 1;
		uint file_end	= file.size();

		file = file.substr(file_start, file_end);

		return file;
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

void C_Material::SetTextureId(const uint& texture_id)
{
	if (material != nullptr)
	{
		material->tex_data.id = texture_id;
	}
}

void C_Material::GetTextureSize(uint& width, uint& height)
{
	if (material != nullptr)
	{
		width	= material->tex_data.width;
		height	= material->tex_data.height;
	}
}

bool C_Material::UseDefaultTexture() const
{
	return use_default_tex;
}

void C_Material::SetUseDefaultTexture(const bool& set_to)
{
	use_default_tex = set_to;
}
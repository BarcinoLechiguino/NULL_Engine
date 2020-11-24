#include "Color.h"

#include "GameObject.h"
#include "R_Material.h"
#include "R_Texture.h"

#include "C_Material.h"

C_Material::C_Material(GameObject* owner) : Component(owner, COMPONENT_TYPE::MATERIAL, "Material"),
material(nullptr),
texture(nullptr),
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

		delete material;
		material = nullptr;
	}

	if (texture != nullptr)
	{
		texture->CleanUp();

		delete texture;
		texture = nullptr;
	}

	for (uint i = 0; i < textures.size(); ++i)
	{
		textures[i]->CleanUp();											// FIX: nullptr R_Textures* can be found inside.

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

R_Texture* C_Material::GetTexture() const
{
	return texture;
}

void C_Material::SetMaterial(R_Material* r_material)
{
	material = r_material;
}

void C_Material::SetTexture(R_Texture* r_texture)
{
	texture = r_texture;
}

Color C_Material::GetMaterialColour()
{
	if (material != nullptr)
	{
		return material->diffuse_color;
	}

	return Color();
}

void C_Material::SetMaterialColour(const Color& color)
{
	material->diffuse_color = color;
}

// --- MATERIAL AND TEXTURE GET/SET DATA METHODS
void C_Material::SetMaterialColour(float r, float g, float b, float a)
{
	material->diffuse_color = Color(r, g, b, a);
}

std::string C_Material::GetTexturePath() const
{
	if (texture != nullptr)
	{
		return texture->tex_data.path;
	}

	return "NONE";
}

std::string C_Material::GetTextureFile() const
{
	if (texture != nullptr)
	{
		std::string file = texture->tex_data.path;

		uint file_start = file.find_last_of("/") + 1;
		uint file_end	= file.size();

		file = file.substr(file_start, file_end);

		return file;
	}

	return "NONE";
}

uint C_Material::GetTextureId() const
{
	if (texture != nullptr)
	{
		return texture->tex_data.id;
	}

	return 0;
}

void C_Material::SetTextureId(const uint& texture_id)
{
	if (material != nullptr)
	{
		texture->tex_data.id = texture_id;
	}
}

void C_Material::GetTextureSize(uint& width, uint& height)
{
	if (texture != nullptr)
	{
		width	= texture->tex_data.width;
		height	= texture->tex_data.height;
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
#include "VarTypedefs.h"
#include "Macros.h"

#include "Color.h"
#include "Configuration.h"

#include "R_Material.h"
#include "R_Texture.h"

#include "GameObject.h"

#include "C_Material.h"

#define MAX_MAPS 7

C_Material::C_Material(GameObject* owner) : Component(owner, COMPONENT_TYPE::MATERIAL),
material			(nullptr),
texture				(nullptr),
use_default_tex		(false)
{
	//memset(texture_maps, 0, sizeof(uint) * MAX_MAPS);
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

		RELEASE(material);
	}

	if (texture != nullptr)
	{
		texture->CleanUp();

		RELEASE(texture);
	}

	/*for (uint i = 0; i < textures.size(); ++i)
	{
		textures[i]->CleanUp();											// FIX: nullptr R_Textures* can be found inside.

		delete textures[i];
		textures[i] = nullptr;
	}

	textures.clear();*/

	return ret;
}

bool C_Material::SaveConfiguration(Configuration& configuration) const
{
	bool ret = true;



	return ret;
}

bool C_Material::LoadConfiguration(Configuration& configuration)
{
	bool ret = true;



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
	if (texture != nullptr)												// Change later to tex_id array.
	{
		texture->CleanUp();
		
		RELEASE(texture);
	}

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
	if (material != nullptr)
	{
		material->diffuse_color = Color(r, g, b, a);
	}
	else
	{
		LOG("[ERROR] Material Component of %s has no Material Resource!", owner->GetName());
	}
}

void C_Material::AddTextureMap(TEXTURE_MAP texture_map, uint tex_id)
{
	//texture_maps[(uint)texture_map] = tex_id;
}

void C_Material::SetCurrentTextureMap(TEXTURE_MAP texture_map)
{
	// current_map = texture_map;
	// tex_id		= texture_maps[(uint)current_map];
}

bool C_Material::UseDefaultTexture() const
{
	return use_default_tex;
}

void C_Material::SetUseDefaultTexture(const bool& set_to)
{
	use_default_tex = set_to;
}

const char* C_Material::GetTexturePath() const
{
	return (texture != nullptr) ? texture->GetAssetsPath() : "NONE";
}

const char* C_Material::GetTextureFile() const
{
	return (texture != nullptr) ? texture->GetAssetsFile() : "NONE";
}

uint C_Material::GetTextureID() const
{
	return (texture != nullptr) ? texture->GetTextureID() : 0;
}

void C_Material::GetTextureSize(uint& width, uint& height)
{
	if (texture != nullptr)
	{
		width	= texture->GetTextureWidth();
		height	= texture->GetTextureHeight();
	}
	else
	{
		LOG("[ERROR] Material Component of %s has no Texture Resource!", owner->GetName());
	}
}

void C_Material::GetTextureInfo(uint& id, uint& width, uint& height, uint& depth, uint& bpp, uint& bytes, std::string& format, bool& compressed)
{
	if (texture != nullptr)
	{
		id			= texture->GetTextureID();
		width		= texture->GetTextureWidth();
		height		= texture->GetTextureHeight();
		depth		= texture->GetTextureDepth();
		bpp			= texture->GetTextureBpp();
		bytes		= texture->GetTextureBytes();
		format		= texture->GetTextureFormatString();
		compressed	= texture->TextureIsCompressed();
	}
	else
	{
		//LOG("[ERROR] Material Component of %s has no Texture Resource!", owner->GetName());
	}
}
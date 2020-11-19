#ifndef __C_MATERIAL_H__
#define __C_MATERIAL_H__

#include <vector>

#include "Color.h"

#include "Component.h"

class GameObject;
class R_Material;
class R_Texture;

struct Texture;

enum class TEX_MAP													// Will correspond with the indexes of the texture_ids vector. Ex: texture_ids[DIFFUSE] = diffuse tex_id;
{
	DEFAULT		= 0,												// Will correspond with the Debug Texture (Checkers).
	DIFFUSE		= 1,
	SPECULAR	= 2,
	AMBIENT		= 3,
	EMISSIVE	= 4,
	HEIGHT		= 5,
	NORMAL		= 6,
	UNKNOWN		= -1
};

class C_Material : public Component
{
public:
	C_Material(GameObject* owner);
	~C_Material();

	bool Update() override;
	bool CleanUp() override;

public:
	R_Material*		GetMaterial() const;
	R_Texture*		GetTexture() const;

	void			SetMaterial(R_Material* material);
	void			SetTexture(R_Texture* texture);

public:
	Color			GetMaterialColour();
	void			SetMaterialColour(const Color& new_colour);
	void			SetMaterialColour(float r, float g, float b, float a);
	
	std::string		GetTexturePath() const;
	std::string		GetTextureFile() const;
	uint			GetTextureId() const;
	void			GetTextureSize(uint& width, uint& height);

	void			SetTextureId(const uint& texture_id);

	bool			UseDefaultTexture() const;
	void			SetUseDefaultTexture(const bool& set_to);

public:
	std::vector<R_Texture*>		textures;											// Will store all the textures that this component can have.
	//std::vector<uint>			texture_ids;										// Will store all the Texture Id's related with this Material Component.
	//uint texture_ids[7];															// { 0, 0, 0, 0, 0, 0, 0 };

private:
	R_Material*					material;											// Currently used material.
	R_Texture*					texture;

	bool						use_default_tex;
	bool						use_albedo_tex;
};

#endif // !__C_MATERIAL_H__
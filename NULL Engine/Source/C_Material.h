#ifndef __C_MATERIAL_H__
#define __C_MATERIAL_H__

#include <vector>

#include "Color.h"

#include "Component.h"

class GameObject;
class R_Material;
class R_Texture;

struct Texture;

class C_Material : public Component
{
public:
	C_Material(GameObject* owner);
	~C_Material();

	bool Update() override;
	bool CleanUp() override;

public:
	R_Material*		GetMaterial() const;
	void			SetMaterial(R_Material* material);

	void			SetMaterialColour(const Color& new_colour);
	void			SetMaterialColour(const float& r, const float& g, const float& b, const float& a);
	
	std::string		GetTexturePath() const;
	std::string		GetTextureFile() const;
	uint			GetTextureId() const;
	void			GetTextureSize(uint& width, uint& height);

	void			SetTextureId(const uint& texture_id);

	bool			UseDefaultTexture() const;
	void			SetUseDefaultTexture(const bool& set_to);

public:
	std::vector<R_Material*>	textures;							// Will store all the textures that this component can have.

	bool						use_default_tex;

private:
	R_Material*					material;							// Currently used material.
	R_Texture*					texture;
};

#endif // !__C_MATERIAL_H__
#ifndef __C_MATERIAL_H__
#define __C_MATERIAL_H__

#include "Component.h"

class GameObject;
class R_Material;

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

	std::string		GetTexturePath() const;
	uint			GetTextureId() const;
	void			GetTextureSize(uint& width, uint& height);

	void			SetTextureId(const uint& texture_id);

public:
	std::vector<R_Material*> textures;								// Will store all the textures that this component can have.

private:
	R_Material* material;											// Currently used material.
};

#endif // !__C_MATERIAL_H__
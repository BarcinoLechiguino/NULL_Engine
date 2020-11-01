#ifndef __R_MATERIAL_H__
#define __R_MATERIAL_H__

#include "Resource.h"

enum class TEXTURE_TYPE									// The enum values are set according to the values of Assimp's aiTextureType enum.
{
	NONE		= 0x0,
	DIFFUSE		= 0x1,
	SPECULAR	= 0x2,
	UNKNOWN		= 0xC
};

struct Texture
{
	Texture();

	std::string		path;								// Path of the file in the directory. Will be used to avoid making duplicates.
	TEXTURE_TYPE	type;								// Diffuse or Specular.
	uint			id;									// Id of the texture.
	uint			width;								// Width of the texture in pixels.
	uint			height;								// Height of the texture in pixels.
};

class R_Material : public Resource
{
public:
	R_Material();
	~R_Material();

	bool CleanUp();

public:
	Texture tex_data;

private:
	
};

#endif // !__R_MATERIAL_H__
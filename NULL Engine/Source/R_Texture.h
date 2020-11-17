#ifndef __R_TEXTURE_H__
#define __R_TEXTURE_H__

#include "Resource.h"

/*struct Texture
{
	Texture();

	std::string		path;								// Path of the file in the directory. Will be used to avoid making duplicates.
	std::string		file;								// File and extension string of the texture. Maybe use path + App->file_system->GetFileAndExtension() instead?
	TEXTURE_TYPE	type;								// Diffuse or Specular.
	uint			id;									// Id of the texture.
	uint			width;								// Width of the texture in pixels.
	uint			height;								// Height of the texture in pixels.
};*/

struct Texture;

class R_Texture : public Resource
{
public:
	R_Texture();
	~R_Texture();

	bool CleanUp() override;

public:
	//Texture tex_data;

private:

};

#endif // !__R_TEXTURE_H__
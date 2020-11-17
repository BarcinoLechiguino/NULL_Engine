#ifndef __R_TEXTURE_H__
#define __R_TEXTURE_H__

#include "Resource.h"

class R_Texture : public Resource
{
public:
	R_Texture();
	~R_Texture();

	bool CleanUp() override;

public:


private:

};

#endif // !__R_TEXTURE_H__
#ifndef __R_MATERIAL_H__
#define __R_MATERIAL_H__

#include "Resource.h"

class R_Material : public Resource
{
public:
	R_Material();
	~R_Material();

public:
	const char* path;
	uint width;
	uint height;

	uint id;

private:
	
};

#endif // !__R_MATERIAL_H__
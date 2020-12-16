#ifndef __R_MODEL_H__
#define __R_MODEL_H__

#include "Resource.h"

class	Resources;

class R_Model : public Resource
{
public:
	R_Model();
	~R_Model();



public:
	std::vector<Resources*> model_resources;
};

#endif // !__R_MODEL_H__
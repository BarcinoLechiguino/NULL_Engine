#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <vector>
#include <string>

#include "Log.h"
#include "VarTypedefs.h"
#include "MathStructures.h"

class Resource
{
public:
	Resource();
	virtual ~Resource();

	virtual bool CleanUp();

private:

};

#endif // !__RESOURCE_H__
#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <vector>
#include <string>

#include "parson/parson.h"


class Configuration
{
public:
	Configuration();
	Configuration(const char* buffer);
	Configuration(JSON_Object* object);
	~Configuration();

	bool Release();									// Will free the memory allocated to the parson elements.

public:

	// --- Get

	// -------

	// --- Set

	// -------

private:
	JSON_Value*		root_value;						// First value of a given parsed file. The first JSON Object will be derived from this value.
	JSON_Object*	node;							// A JSON Object is the same as an XML Node. Main node that will from which the rest will be derived.
};

#endif // !__CONFIGURATION_H__
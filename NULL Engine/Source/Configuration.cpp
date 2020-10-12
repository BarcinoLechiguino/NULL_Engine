// ----------------------------------------------------
// Interface module for parson. (Wrapper)
// Employed to load/save engine configurations.
// ----------------------------------------------------

#include "Globals.h"

#include "Configuration.h"

Configuration::Configuration()
{
	root_value = json_value_init_object();								// Creates an empty JSON_Object. As it is a JSON_Value*, it needs to be passed to a JSON_Object*
	node = json_value_get_object(root_value);							// Gets the previous JSON_Value and sets it as a JSON_Object*. Will be the first handle/node of the file.
}

Configuration::Configuration(const char* buffer)
{
	root_value = json_parse_string(buffer);								// Creates a JSON_Object out of a given buffer. Used to load a previous configuration.
	
	if (root_value != NULL)												// Using NULL instead of nullptr as parson is a library written in C, not C++.
	{
		node = json_value_get_object(root_value);
	}
	else
	{
		LOG("Parse operation through string failed! root_value was NULL");
	}
}

Configuration::Configuration(JSON_Object* object) : node(object)		// If config is initialized with an already existing JSON_Object*, node will be directly initialized with it.
{
	
}

Configuration::~Configuration()
{
	Release();															// Will clean up any allocated memory in this specific Configuration().
}

bool Configuration::Release()
{
	if (root_value != NULL)
	{
		json_value_free(root_value);									// Frees the memory that parson previously allocated to root_value.
	}

	return true;
}
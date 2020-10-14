// ----------------------------------------------------
// Interface module for parson. (Wrapper)
// Employed to load/save engine configurations.
// ----------------------------------------------------

#include "Globals.h"

#include "Configuration.h"

Configuration::Configuration() : root_value(nullptr), root_node(nullptr)
{
	root_value = json_value_init_object();								// Creates an empty JSON_Object. As it is a JSON_Value*, it needs to be passed to a JSON_Object*
	root_node = json_value_get_object(root_value);						// Gets the previous JSON_Value and sets it as a JSON_Object*. Will be the first handle/node of the file.
}

Configuration::Configuration(const char* buffer) : root_value(nullptr), root_node(nullptr)
{
	root_value = json_parse_string(buffer);								// Creates a JSON_Object out of a given buffer. Used to load a previous configuration.
	
	if (root_value != NULL)												// Using NULL instead of nullptr as parson is a library written in C, not C++.
	{
		root_node = json_value_get_object(root_value);
	}
	else
	{
		LOG("[error] Parse operation through string failed! root_value was NULL");
	}
}

Configuration::Configuration(JSON_Object* object) : root_value(nullptr), root_node(object)	// If config is initialized with an already existing JSON_Object*, node will be directly initialized with it.
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

double Configuration::GetNumber(const char* name) const
{	
	if (json_object_has_value_of_type(root_node, name, JSONNumber))		// First whether or not the value in the child node is of the correct type is checked.
	{
		return json_object_get_number(root_node, name);					// If the type matches the function's return type, then the value is read from the .json file.
	}

	return -1;
}

const char* Configuration::GetString(const char* name) const
{
	if (json_object_has_value_of_type(root_node, name, JSONString))
	{
		return json_object_get_string(root_node, name);
	}

	return "";
}

bool Configuration::GetBool(const char* name) const
{
	if (json_object_has_value_of_type(root_node, name, JSONBoolean))
	{
		return json_object_get_boolean(root_node, name);
	}

	return false;
}

Configuration_Array Configuration::GetArray(const char* name) const
{
	if (json_object_has_value_of_type(root_node, name, JSONArray))
	{
		return Configuration_Array(json_object_get_array(root_node, name));
	}
}

Configuration Configuration::GetNode(const char* name) const
{
	return Configuration(json_object_get_object(root_node, name));
}

void Configuration::SetNumber(const char* name, double number)
{
	json_object_set_number(root_node, name, number);
}

void Configuration::SetString(const char* name, const char* string)
{
	json_object_set_string(root_node, name, string);
}

void Configuration::SetBool(const char* name, bool value)
{
	json_object_set_boolean(root_node, name, value);
}

Configuration_Array Configuration::SetArray(const char* name)
{
	json_object_set_value(root_node, name, json_value_init_array());				// Adding the array to the .json file.

	return Configuration_Array(json_object_get_array(root_node, name));				// Constructing and returning a handle to the created array.
}

Configuration Configuration::SetNode(const char* name)
{
	json_object_set_value(root_node, name, json_value_init_object());				// Adding the node to the .json file.

	return Configuration(json_object_get_object(root_node, name));					// Constructing and returning a handle to the created node.
}

JSON_Value* Configuration::FindValue(const char* name, int index)
{
	if (index < 0)
	{
		return json_object_get_value(root_node, name);
	}
	else
	{
		JSON_Array* json_array = json_object_get_array(root_node, name);

		if (json_array != nullptr)
		{
			return json_array_get_value(json_array, index);
		}
	}
}

uint Configuration::SerializeToBuffer(char** buffer)
{
	uint size = (uint)json_serialization_size_pretty(root_value);
	*buffer = new char[size];
	
	JSON_Status status = json_serialize_to_buffer_pretty(root_value, *buffer, size);

	if (status == JSONFailure)
	{
		LOG("[error] Configuration could not serialize the buffer.")
	}

	return size;
}

// ------------------ CONFIGURATION_ARRAY METHODS ------------------
Configuration_Array::Configuration_Array()
{
	json_array = nullptr;
	size = 0;
}

Configuration_Array::Configuration_Array(JSON_Array* json_array) : json_array(json_array), size(0)
{

}
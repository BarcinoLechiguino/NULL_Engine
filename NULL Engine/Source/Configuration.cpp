// ----------------------------------------------------
// Interface module for parson. (Wrapper)
// Employed to load/save engine configurations.
// ----------------------------------------------------

#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4.h"

#include "VarTypedefs.h"																		// Globals										
#include "Log.h"																				// -------

#include "Color.h"																				// Containers

#include "Configuration.h"																		// Header file of this cpp file.

ParsonNode::ParsonNode() : 
root_value	(nullptr), 
root_node	(nullptr)
{
	root_value	= json_value_init_object();														// Creates an empty JSON_Object. Being a JSON_Value*, it needs to be passed to a JSON_Object*
	root_node	= json_value_get_object(root_value);											// Gets the previous JSON_Value and sets it as a JSON_Object*. First handle/node of the file.
}

ParsonNode::ParsonNode(const char* buffer) : 
root_value	(nullptr), 
root_node	(nullptr)
{
	root_value = json_parse_string(buffer);														// Creates a JSON_Object out of a given buffer. Used to load a previous configuration.
	
	if (root_value != NULL)																		// Using NULL instead of nullptr as parson is a library written in C, not C++.
	{
		root_node = json_value_get_object(root_value);
	}
	else
	{
		LOG("[ERROR] Parse operation through string failed!");
	}
}

ParsonNode::ParsonNode(JSON_Object* object) :												// If config is init with an existing JSON_Object*, node will be directly init with it.
root_value	(nullptr), 
root_node	(object)
{
	
}

ParsonNode::~ParsonNode()
{
	Release();																					// Will clean up any allocated memory in this specific Configuration().
}

uint ParsonNode::SerializeToBuffer(char** buffer)
{
	uint size = (uint)json_serialization_size_pretty(root_value);
	*buffer = new char[size];

	JSON_Status status = json_serialize_to_buffer_pretty(root_value, *buffer, size);

	if (status == JSONFailure)
	{
		LOG("[ERROR] Could not serialize the buffer! Error: Parson could not allocate or write the buffer.");
	}

	return size;
}

bool ParsonNode::Release()
{
	if (root_value != NULL)
	{
		json_value_free(root_value);															// Frees the memory that parson previously allocated to root_value.
	}

	return true;
}

// --- CONFIGURATION METHODS ---
// --- GETTERS AND SETTERS
double ParsonNode::GetNumber(const char* name) const
{	
	if (NodeHasValueOfType(name, JSONNumber))														// First whether or not the value in the child node is of the correct type is checked.
	{
		return json_object_get_number(root_node, name);											// If the type matches the function's return type, then the value is read from the .json file.
	}
	else
	{
		const char* node_name = json_object_get_name(root_node, 0);
		LOG("[ERROR] Configuration: Node %s's %s did not have a Number variable!", node_name, name);
	}

	return JSONError;
}

const char* ParsonNode::GetString(const char* name) const
{
	if (NodeHasValueOfType(name, JSONString))
	{
		return json_object_get_string(root_node, name);
	}
	else
	{
		LOG("[ERROR] Configuration: %s did not have a String variable!", name);
	}

	return "NOT FOUND";
}

bool ParsonNode::GetBool(const char* name) const
{
	if (NodeHasValueOfType(name, JSONBoolean))
	{
		return json_object_get_boolean(root_node, name);
	}
	else
	{
		LOG("[ERROR] Configuration: %s did not have a Bool variable!", name);
	}

	return false;
}

ParsonArray ParsonNode::GetArray(const char* name) const
{
	if (NodeHasValueOfType(name, JSONArray))
	{
		return ParsonArray(json_object_get_array(root_node, name));
	}
	else
	{
		LOG("[ERROR] Configuration: %s did not have a JSON_Array variable!", name);
	}

	return ParsonArray();																// If an empty config array is returned then check that json_array == nullptr and size == 0.
}

ParsonNode ParsonNode::GetNode(const char* name) const
{
	if (!NodeHasValueOfType(name, JSONObject))
	{
		LOG("[ERROR] Configuration: %s did not have a JSON_Object variable!", name);			// Just for display purposes.
	}
	
	return ParsonNode(json_object_get_object(root_node, name));								// json_object_get_object() returns NULL if no JSON_Object can be found. Remember to check!
}

void ParsonNode::SetNumber(const char* name, double number)
{
	JSON_Status status = json_object_set_number(root_node, name, number);

	if (status == JSONFailure)
	{
		LOG("[ERROR] Configuration: Could not set %s with the given Number!", name);
	}
}

void ParsonNode::SetString(const char* name, const char* string)
{
	JSON_Status status = json_object_set_string(root_node, name, string);

	if (status == JSONFailure)
	{
		LOG("[ERROR] Configuration: Could not set %s with the given String!", name);
	}
}

void ParsonNode::SetBool(const char* name, bool value)
{
	JSON_Status status = json_object_set_boolean(root_node, name, value);

	if (status == JSONFailure)
	{
		LOG("[ERROR] Configuration: Could not set %s with the given Bool!", name);
	}
}

ParsonArray ParsonNode::SetArray(const char* name)
{
	JSON_Status status = json_object_set_value(root_node, name, json_value_init_array());		// Adding the array to the .json file.

	if (status == JSONFailure)
	{
		LOG("[ERROR] Configuration: Could not set %s with an JSON_Array!", name);
	}
	
	return ParsonArray(json_object_get_array(root_node, name));									// Constructing and returning a handle to the created array.
}																								// json_object_get_array() will return NULL if the JSON_Array could not be created.

ParsonNode ParsonNode::SetNode(const char* name)
{
	json_object_set_value(root_node, name, json_value_init_object());							// Adding the node to the .json file.

	return ParsonNode(json_object_get_object(root_node, name));									// Constructing and returning a handle to the created node.
}

bool ParsonNode::NodeHasValueOfType(const char* name, JSON_Value_Type value_type) const
{
	return json_object_has_value_of_type(root_node, name, value_type);
}

JSON_Value* ParsonNode::FindValue(const char* name, int index)
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

	return nullptr;
}

// ------------------ CONFIGURATION_ARRAY METHODS ------------------
ParsonArray::ParsonArray()
{
	json_array = nullptr;
	size = 0;
}

ParsonArray::ParsonArray(JSON_Array* json_array) : 
json_array	(nullptr), 
size		(0)
{
	if (json_array != nullptr)
	{
		this->json_array	= json_array;
		size				= json_array_get_count(json_array);
	}
	else
	{
		LOG("[ERROR] The JSON_Array* passed to the constructor was nullptr!");
	}
}

double ParsonArray::GetNumber(const uint& index) const
{
	if (HasValueOfTypeAtIndex(index, JSONNumber))
	{
		return json_array_get_number(json_array, index);
	}
	else
	{
		LOG("[ERROR] Configuration: Array Index %u did not have a Number variable!", index);
	}

	return JSONError;
}

const char* ParsonArray::GetString(const uint& index) const
{
	if (HasValueOfTypeAtIndex(index, JSONString))
	{
		return json_array_get_string(json_array, index);
	}
	else
	{
		LOG("[ERROR] Configuration: Array Index %u did not have a String variable!", index);
	}
	
	return "NOT FOUND";
}

bool ParsonArray::GetBool(const uint& index) const
{
	if (HasValueOfTypeAtIndex(index, JSONBoolean))
	{
		return json_array_get_boolean(json_array, index);
	}
	else
	{
		LOG("[ERROR] Configuration: Array Index %u did not have a Bool variable!", index);
	}

	return false;
}

void ParsonArray::GetColor(const uint& index, Color& color) const
{
	bool valid_red		= HasValueOfTypeAtIndex(index, JSONNumber);
	bool valid_green	= HasValueOfTypeAtIndex(index + 1, JSONNumber);
	bool valid_blue		= HasValueOfTypeAtIndex(index + 2, JSONNumber);
	bool valid_alpha	= HasValueOfTypeAtIndex(index + 3, JSONNumber);

	if (valid_red && valid_green && valid_blue && valid_alpha)
	{
		color.r = json_array_get_number(json_array, index);
		color.g = json_array_get_number(json_array, index + 1);
		color.b = json_array_get_number(json_array, index + 2);
		color.a = json_array_get_number(json_array, index + 3);
	}
}

void ParsonArray::GetFloat3(const uint& index, float3& vec3) const
{
	bool valid_x = HasValueOfTypeAtIndex(index, JSONNumber);
	bool valid_y = HasValueOfTypeAtIndex(index + 1, JSONNumber);
	bool valid_z = HasValueOfTypeAtIndex(index + 2, JSONNumber);

	if (valid_x && valid_y && valid_z)
	{
		vec3.x = json_array_get_number(json_array, index);
		vec3.y = json_array_get_number(json_array, index + 1);
		vec3.z = json_array_get_number(json_array, index + 2);
	}
}

void ParsonArray::GetFloat4(const uint& index, float4& vec4) const
{
	bool valid_x = HasValueOfTypeAtIndex(index, JSONNumber);
	bool valid_y = HasValueOfTypeAtIndex(index + 1, JSONNumber);
	bool valid_z = HasValueOfTypeAtIndex(index + 2, JSONNumber);
	bool valid_w = HasValueOfTypeAtIndex(index + 3, JSONNumber);

	if (valid_x && valid_y && valid_z && valid_w)
	{
		vec4.x = json_array_get_number(json_array, index);
		vec4.y = json_array_get_number(json_array, index + 1);
		vec4.z = json_array_get_number(json_array, index + 2);
		vec4.w = json_array_get_number(json_array, index + 3);
	}
}

ParsonNode ParsonArray::GetNode(const uint& index) const
{
	if (HasValueOfTypeAtIndex(index, JSONObject))
	{
		return json_array_get_object(json_array, index);
	}
}

void ParsonArray::SetNumber(const char* name, const double& number)
{
	JSON_Status status = json_array_append_number(json_array, number);

	if (status == JSONFailure)
	{
		LOG("[ERROR] Configuration: Could not append Number %s to the Array!", name);
	}
}

void ParsonArray::SetString(const char* name, const char* string)
{
	JSON_Status status = json_array_append_string(json_array, string);

	if (status == JSONFailure)
	{
		LOG("[ERROR] Configuration: Could not append String %s to the Array!", name);
	}
}

void ParsonArray::SetBool(const char* name, const bool& value)
{
	JSON_Status status = json_array_append_boolean(json_array, value);

	if (status == JSONFailure)
	{
		LOG("[ERROR] Configuration: Could not append Boolean %s to the Array!", name);
	}
}

void ParsonArray::SetColor(const char* name, const Color& color)
{

}

void ParsonArray::SetFloat3(const char* name, const math::float3& vec3)
{

}

void ParsonArray::SetFloat4(const char* name, const math::float4& vec4)
{

}

ParsonNode ParsonArray::SetNode(const char* name)
{
	return ParsonNode();
}

JSON_Value_Type ParsonArray::GetTypeAtIndex(const uint& index) const
{
	return json_value_get_type(json_array_get_value(json_array, index));
}

bool ParsonArray::HasValueOfTypeAtIndex(const uint& index, JSON_Value_Type value_type) const
{
	if (GetTypeAtIndex(index) == value_type)
	{
		return true;
	}
	
	return false;
}
#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <vector>
#include <string>

#include "Globals.h"

#include "parson/parson.h"

class Configuration_Array;

class Configuration
{
public:
	Configuration();
	Configuration(const char* buffer);
	Configuration(JSON_Object* object);
	~Configuration();

public:
	// --- Get
	double GetNumber(const char* name) const;						//
	const char* GetString(const char* name) const;					//
	bool GetBool(const char* name) const;							//
	Configuration_Array GetArray(const char* name) const;			//
	Configuration GetNode(const char* name) const;					//
	// -------

	// --- Set
	void SetNumber(const char* name, double number);				//
	void SetString(const char* name, const char* string);			//
	void SetBool(const char* name, bool value);						//
	Configuration_Array SetArray(const char* name);					//
	Configuration SetNode(const char* name);						//
	// -------

	// --- Utilities
	JSON_Value* FindValue(const char* name, int index);				//
	uint SerializeToBuffer(char** buffer);							//
	bool Release();													// Will free the memory allocated to the parson elements.
	// -------------

private:
	JSON_Value*		root_value;										// First value of a given parsed file. The first JSON Object will be derived from this value.
	JSON_Object*	root_node;										// A JSON Object is the same as an XML Node. Main node that will from which the rest will be derived.
};

class Configuration_Array
{
public:
	Configuration_Array();
	Configuration_Array(JSON_Array* json_array);

public:
	// --- Get
	double GetNumber(const char* name, uint index) const;						//
	const char* GetString(const char* name, uint index) const;					//
	bool GetBool(const char* name, uint index) const;							//
	Configuration GetNode(const char* name, uint index) const;					//
	// -------

	// --- Set
	void SetNumber				(const char* name, JSON_Value* number);			//
	void SetString				(const char* name, const char* string);			//
	void SetBool				(const char* name, bool value);					//
	Configuration SetNode		(const char* name);								//
	// -------

	// --- Utilities
	uint GetSize() const;
	// -------------

public:
	JSON_Array*		json_array;
	uint			size;
};
#endif // !__CONFIGURATION_H__
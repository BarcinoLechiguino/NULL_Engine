#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>

enum class COMPONENT_TYPE
{
	NONE,
	TRANSFORM,
	MESH,
	MATERIAL,
	LIGHT,
	UNKNOWN = -1
};

class Component
{
public:
	Component(COMPONENT_TYPE type);
	~Component();

public:
	COMPONENT_TYPE	type;
	std::string		name;

private:

};

#endif // !__COMPONENT_H__
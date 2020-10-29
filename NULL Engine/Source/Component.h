#ifndef __COMPONENT_H__
#define __COMPONENT_H__

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
	COMPONENT_TYPE type;

private:

};

#endif // !__COMPONENT_H__
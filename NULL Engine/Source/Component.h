#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>

class GameObject;

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
	Component(GameObject* owner, COMPONENT_TYPE type, const char* name, bool is_active = true);
	virtual ~Component();

	virtual bool Update();
	virtual bool CleanUp();

public:
	const char* GetName() const;
	void SetName(const char* new_name);

public:
	COMPONENT_TYPE	type;

	GameObject* owner;

	bool		is_active;

private:
	const char* name;
};

#endif // !__COMPONENT_H__
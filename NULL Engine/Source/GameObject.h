#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <vector>
#include <map>
#include <string>

typedef unsigned int uint;

class Component;

enum class COMPONENT_TYPE;

class GameObject
{
public:
	GameObject();
	GameObject(uint id = 0, std::string name = "GameObject", bool is_active = true, bool is_static = false);
	~GameObject();

	bool		Update();
	bool		CleanUp();

public:
	bool		AddChild(GameObject* child);											// Adds the given child to the GO's childs vector. Also deletes it from the previous parent's childs.
	bool		DeleteChild(GameObject* child);											// Deletes the given child from the childs vector. Returns false upon not finding the child.
	
	Component*	CreateComponent(COMPONENT_TYPE type);									// Creates a component of the given type and adds it to the components vector.
	Component*	GetComponent(COMPONENT_TYPE type);										// Iterates through the components vector and returns the component that matches the given type.
	const char* GetComponentNameFromType(COMPONENT_TYPE type);							// Returns a string matching the given type. Mainly used for debug purposes (LOG).

	void		SetID(uint id);

public:
	std::vector<Component*>		components;
	std::vector<GameObject*>	childs;

	GameObject* parent;

	std::string name;

	bool is_active;
	bool is_static;

	//mat4x4 transform;

private:
	uint id;
};

#endif // !__GAME_OBJECT_H__
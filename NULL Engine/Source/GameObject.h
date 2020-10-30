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

	bool Update();

	bool CleanUp();

public:
	bool AddChild(GameObject* child);
	bool DeleteChild(GameObject* child);
	Component* CreateComponent(COMPONENT_TYPE type);

	void SetID(uint id);

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
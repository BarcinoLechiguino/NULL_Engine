#ifndef __E_HIERARCHY_H__
#define __E_HIERARCHY_H__

#include "E_Panel.h"

class GameObject;

class E_Hierarchy : public E_Panel
{
public:
	E_Hierarchy();
	~E_Hierarchy();

	bool Draw(ImGuiIO& io) override;

	bool CleanUp() override;

public:
	void PrintGameObjectsOnHierarchy();
	void ProcessGameObject(GameObject* game_object, ImGuiTreeNodeFlags node_flags);
	void ProcessChilds(GameObject* game_object);

private:
	int a;
};

#endif // !__E_HIERARCHY_H__
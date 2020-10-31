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
	void ProcessGameObject(GameObject* game_object);

	void HierarchyToolsPopup();

private:
	ImGuiTreeNodeFlags	default_flags;								// Will define the base configuration of the tree nodes.
	GameObject*			dragged_game_object;						// Used for the dragging and dropping of game objects withing the hierarchy.

	bool				open_hierarchy_tools_popup;					//
};

#endif // !__E_HIERARCHY_H__
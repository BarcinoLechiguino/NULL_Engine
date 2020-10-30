#include "Application.h"
#include "M_SceneIntro.h"

#include "GameObject.h"

#include "E_Hierarchy.h"

E_Hierarchy::E_Hierarchy() : E_Panel("Hierarchy")
{

}

E_Hierarchy::~E_Hierarchy()
{

}

bool E_Hierarchy::Draw(ImGuiIO& io)
{
	bool ret = true;

	ImGui::Begin("Hierarchy");

	PrintGameObjectsOnHierarchy();

	ImGui::End();

	return ret;
}

bool E_Hierarchy::CleanUp()
{
	bool ret = true;

	return ret;
}

void E_Hierarchy::PrintGameObjectsOnHierarchy()
{
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	
	ProcessGameObject(App->scene_intro->root_object, node_flags); 
}

void E_Hierarchy::ProcessGameObject(GameObject* game_object, ImGuiTreeNodeFlags node_flags)
{	
	if (game_object->childs.empty())
	{
		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

		ImGui::TreeNodeEx(game_object->name.c_str(), node_flags);
	}
	else
	{
		if (ImGui::TreeNode(game_object->name.c_str()))
		{
			for (uint i = 0; i < game_object->childs.size(); ++i)
			{
				ProcessGameObject(game_object->childs[i], node_flags);
			}

			ImGui::TreePop();
		}
	}
}

void E_Hierarchy::ProcessChilds(GameObject* game_object)
{

}
#include "Application.h"
#include "M_Editor.h"
#include "M_SceneIntro.h"

#include "GameObject.h"

#include "E_Hierarchy.h"

E_Hierarchy::E_Hierarchy() : E_Panel("Hierarchy"), dragged_game_object(nullptr), open_hierarchy_tools_popup(false)
{
	default_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
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
	if (App->scene_intro->root_object != nullptr)
	{
		if (open_hierarchy_tools_popup)
		{
			HierarchyToolsPopup();
		}
		
		ProcessGameObject(App->scene_intro->root_object);
	}
}

void E_Hierarchy::ProcessGameObject(GameObject* game_object)
{	
	// ------ Setting the tree node's color. ------
	ImVec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	if (!game_object->IsActive())														// If the given game object is not active, the text of the tree node will be displayed in GREY.
	{
		color = { 0.5f, 0.5f, 0.5f, 1.0f };
	}
	
	ImGui::PushStyleColor(ImGuiCol_Text, color);
	// --------------------------------------------

	ImGuiTreeNodeFlags node_flags = default_flags;

	if (game_object->childs.empty())
	{
		node_flags |= ImGuiTreeNodeFlags_Leaf;
	}

	if (game_object == App->editor->GetInspectedGameObject())
	{
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}

	if (game_object == App->scene_intro->root_object)
	{
		node_flags |= ImGuiTreeNodeFlags_DefaultOpen;
	}

	if (ImGui::TreeNodeEx(game_object->GetName(), node_flags))
	{
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))								// IsItemClicked checks if the previous item was clicked. Arguments: 0 (Right Click), 1 (Left Click).
		{
			App->editor->SetInspectedGameObject(game_object);
		}

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			App->editor->SetInspectedGameObject(game_object);
			open_hierarchy_tools_popup = true;
		}

		if (ImGui::BeginDragDropSource())												// First, it is checked whether or not this node is part of a currently starting drag&drop operation.
		{
			ImGui::SetDragDropPayload("DRAGGED_NODE", game_object, sizeof(GameObject));	// Here the payload is being constructed. It can be later identified through the given string.
			ImGui::Text("Dragging %s", game_object->GetName());							// This specific text, as it is within the DragDropSource, will accompany the dragged node.
			dragged_game_object = game_object;											// The game object that will be dragged needs to be saved to be later re-integrated into the hierarchy.

			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())												// Here it is checked whether or not an element is being dropped into this specific node/item.
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DRAGGED_NODE"))	// First, the payload that is being dropped needs to be checked to make sure its the correct one.
			{	
				game_object->AddChild(dragged_game_object);								// (GameObject*)payload->Data would also work. However, it easily breaks, at least in my case.

				dragged_game_object = nullptr;
			}
			
			ImGui::EndDragDropTarget();
		}

		if (!game_object->childs.empty())
		{
			for (uint i = 0; i < game_object->childs.size(); ++i)
			{
				ProcessGameObject(game_object->childs[i]);
			}
		}

		ImGui::TreePop();
	}

	ImGui::PopStyleColor();
}

void E_Hierarchy::HierarchyToolsPopup()
{
	ImGui::OpenPopup("Hierarchy Tools");
	if (ImGui::BeginPopup("Hierarchy Tools"))
	{
		if (ImGui::MenuItem("Delete Selected"))
		{
			if (App->editor->GetInspectedGameObject() != App->scene_intro->root_object)
			{
				App->scene_intro->DeleteGameObject(App->editor->GetInspectedGameObject());
				App->editor->SetInspectedGameObject(nullptr);
				open_hierarchy_tools_popup = false;
			}
			else
			{
				LOG("[WARNING] The Scene's Root Object cannot be deleted!");
			}
		}

		if (ImGui::MenuItem("Create Empty Child GameObject"))
		{
			App->scene_intro->CreateGameObject("Empty Child", App->editor->GetInspectedGameObject());
			open_hierarchy_tools_popup = false;
		}

		ImGui::EndPopup();
	}

	if (ImGui::IsMouseReleased(0))
	{
		open_hierarchy_tools_popup = false;
	}
}
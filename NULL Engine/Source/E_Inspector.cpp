#include "E_Inspector.h"

E_Inspector::E_Inspector() : E_Panel("Inspector")
{

}

E_Inspector::~E_Inspector()
{

}

bool E_Inspector::Draw(ImGuiIO& io)
{
	bool ret = true;

	ImGui::Begin("Inspector");

	ImGui::End();

	return ret;
}

bool E_Inspector::CleanUp()
{
	bool ret = true;

	return ret;
}

void E_Inspector::SetSelectedGameObject(GameObject* game_object)
{
	if (game_object != selected_game_object)
	{
		selected_game_object = game_object;
	}
}

GameObject* E_Inspector::GetSelectedGameObject() const
{
	return selected_game_object;
}

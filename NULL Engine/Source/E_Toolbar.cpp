#include "Application.h"
#include "M_Editor.h"

#include "E_Toolbar.h"

E_Toolbar::E_Toolbar() : EditorPanel("Toolbar")
{

}

E_Toolbar::~E_Toolbar()
{

}

bool E_Toolbar::Draw(ImGuiIO& io)
{
	bool ret = true;

	ImGui::Begin("Toolbar");

	PlayAndStopButtons();
	ImGui::SameLine();
	TimeDisplays();

	ImGui::End();

	return ret;
}

bool E_Toolbar::CleanUp()
{
	bool ret = true;



	return ret;
}

void E_Toolbar::PlayAndStopButtons()
{
	if (ImGui::Button("Play"))
	{
		App->editor->SaveSceneThroughEditor("PlayAutosave");
		//App->in_game_mode = true;
		App->play = true;
	}

	ImGui::SameLine();

	if (ImGui::Button("Stop"))
	{
		App->editor->LoadFileThroughEditor("Assets/Scenes/PlayAutosave.json");
		//App->in_game_mode = false;
		App->play = false;
	}
}

void E_Toolbar::TimeDisplays()
{
	ImGui::Text("Real Time: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Put Engine Clock Here.");
	ImGui::SameLine();
	ImGui::Text("Game Time: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Put Game Clock Here.");
}
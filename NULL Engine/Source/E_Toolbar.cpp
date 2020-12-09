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

	ImGui::Button("Play");
	ImGui::SameLine();
	ImGui::Button("Stop");

	ImGui::SameLine();

	ImGui::Text("Real Time: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Put Engine Clock Here.");
	ImGui::SameLine();
	ImGui::Text("Game Time: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Put Game Clock Here.");

	ImGui::End();

	return ret;
}

bool E_Toolbar::CleanUp()
{
	bool ret = true;



	return ret;
}
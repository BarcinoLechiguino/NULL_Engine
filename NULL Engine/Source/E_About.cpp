#include "Application.h"

#include "E_About.h"

E_About::E_About() : E_Panel("About", false)
{

}

E_About::~E_About()
{

}

bool E_About::Draw(ImGuiIO& io)
{
	bool ret = true;

	ImGui::Begin(GetName());

	if (ImGui::Button("Delete::"))
	{
		ImGui::OpenPopup("Delete for reals? :(");
		ImGui::EndPopup();
	}

	ImGui::End();

	return ret;
}

bool E_About::CleanUp()
{
	bool ret = true;

	return ret;
}
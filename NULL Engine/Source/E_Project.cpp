#include "E_Project.h"

E_Project::E_Project() : EditorPanel("Project")
{

}

E_Project::~E_Project()
{

}

bool E_Project::Draw(ImGuiIO& io)
{
	bool ret = true;

	ImGui::Begin("Project");



	ImGui::End();

	return ret;
}

bool E_Project::CleanUp()
{
	bool ret = true;



	return ret;
}
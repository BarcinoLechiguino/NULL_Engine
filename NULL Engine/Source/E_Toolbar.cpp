#include "Application.h"
#include "M_Editor.h"

#include "E_Test.h"
#include "E_About.h"
#include "E_Console.h"

#include "E_Toolbar.h"

E_Toolbar::E_Toolbar() : E_Panel("Toolbar")
{

}

E_Toolbar::~E_Toolbar()
{

}

bool E_Toolbar::Draw(ImGuiIO& io)
{
	bool ret = true;

	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Quit Application"))
		{
			return false;
			
			/*ImGui::OpenPopup("Close Application?");

			if (ImGui::Button("YES"))
			{
				return false;
			}

			ImGui::SameLine();

			if (ImGui::Button("NO"))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();*/
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("View"))
	{
		if (ImGui::MenuItem("Console"))
		{
			if (App->editor->console->IsActive())
			{
				App->editor->console->Disable();
			}
			else
			{
				App->editor->console->Enable();
			}
		}

		if (ImGui::MenuItem("Configuration"))
		{

		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("GuiDemo"))
		{
			if (App->editor->test->IsActive())
			{
				App->editor->test->Disable();
			}
			else
			{
				App->editor->test->Enable();
			}
		}

		if (ImGui::MenuItem("Documentation"))
		{
			App->RequestBrowser("https://github.com/BarcinoLechiguino/NULL_Engine");
		}

		if (ImGui::MenuItem("Download Latest"))
		{
			App->RequestBrowser("https://github.com/BarcinoLechiguino/NULL_Engine/tags");
		}

		if (ImGui::MenuItem("Report a Bug"))
		{
			App->RequestBrowser("https://github.com/BarcinoLechiguino/NULL_Engine/issues/new");
		}

		if (ImGui::MenuItem("About"))
		{
			App->editor->about->Draw(io);
		}

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	return ret;
}

bool E_Toolbar::CleanUp()
{
	bool ret = true;

	return ret;
}
#include "Application.h"
#include "M_Editor.h"

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

	FileMainMenuItem();
	WindowMainMenuItem();
	HelpMainMenuItem();

	if (App->editor->show_close_app_popup)
	{
		CloseAppPopup();																	// Not actually inside MainMenuBar but related to FileMainMenuItem().
	}

	ImGui::EndMainMenuBar();

	return ret;
}

bool E_Toolbar::CleanUp()
{
	bool ret = true;

	return ret;
}

// -------- TOOLBAR METHODS --------
bool E_Toolbar::FileMainMenuItem()
{
	bool ret = true;
	
	if (ImGui::BeginMenu("File"))
	{
		ImGui::MenuItem("Quit Application", "ESC", &App->editor->show_close_app_popup);		// MenuItem(Item name string, shortcut string, bool to modify / get modified by)

		ImGui::EndMenu();
	}

	return ret;
}

bool E_Toolbar::WindowMainMenuItem()
{
	bool ret = true;

	if (ImGui::BeginMenu("Window"))
	{
		ImGui::MenuItem("Configuration", "1", &App->editor->show_configuration);
		ImGui::MenuItem("Inspector", "2", &App->editor->show_hierarchy);
		ImGui::MenuItem("Hierarchy", "3", &App->editor->show_inspector);
		ImGui::MenuItem("Console", "4", &App->editor->show_console);

		ImGui::EndMenu();
	}

	return ret;
}

bool E_Toolbar::HelpMainMenuItem()
{
	bool ret = true;

	if (ImGui::BeginMenu("Help"))
	{
		ImGui::MenuItem("GuiDemo", "8", &App->editor->show_imgui_demo);
		ImGui::MenuItem("About", "9", &App->editor->show_about_popup);

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

		ImGui::EndMenu();
	}

	return ret;
}

bool E_Toolbar::CloseAppPopup()
{
	bool ret = true;

	ImGui::OpenPopup("Close Application?");

	if (ImGui::BeginPopupModal("Close Application?"))
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.25f));
		if (ImGui::Button("CONFIRM"))
		{
			ImGui::CloseCurrentPopup();
			App->editor->show_close_app_popup = false;

			App->quit = true;
		}
		ImGui::PopStyleColor();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.25f));
		if (ImGui::Button("CANCEL"))
		{
			ImGui::CloseCurrentPopup();
			App->editor->show_close_app_popup = false;
		}
		ImGui::PopStyleColor();

		ImGui::EndPopup();
	}

	return ret;
}
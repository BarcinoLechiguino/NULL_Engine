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
	EditMainMenuItem();
	WindowMainMenuItem();
	ViewMainMenuItem();
	GameObjectsMainMenuItem();
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
		ImGui::MenuItem("New Scene", "Ctrl+N", nullptr, false);
		ImGui::MenuItem("Open Scene", "Ctrl+O", nullptr, false);

		ImGui::Separator();

		ImGui::MenuItem("Save", "Ctrl+S");
		ImGui::MenuItem("Save As...", "Ctrl+Shift+S", nullptr, false);

		ImGui::Separator();

		ImGui::MenuItem("New Project", "Ctrl+Shift+N", nullptr, false);
		ImGui::MenuItem("Open Project", "Ctrl+Shift+O", nullptr, false);
		ImGui::MenuItem("Save Project", "Ctrl+Alt+S", nullptr, false);

		ImGui::Separator();
		
		ImGui::MenuItem("Quit", "ESC", &App->editor->show_close_app_popup);		// MenuItem(Item name string, shortcut string, bool to modify / get modified by)

		ImGui::EndMenu();
	}

	return ret;
}

bool E_Toolbar::EditMainMenuItem()
{
	bool ret = true;

	if (ImGui::BeginMenu("Edit"))
	{
		ImGui::MenuItem("Undo", "Ctrl+Z", nullptr, false);
		ImGui::MenuItem("Redo", "Ctrl+Y", nullptr, false);

		ImGui::Separator();

		ImGui::MenuItem("Select All", "Ctrl+A", nullptr, false);
		ImGui::MenuItem("Deselect All", "Shift+D", nullptr, false);
		ImGui::MenuItem("Select Children", "Shift+C", nullptr, false);
		ImGui::MenuItem("Invert Selection", "Ctrl+I", nullptr, false);

		ImGui::Separator();

		ImGui::MenuItem("Duplicate", "Ctrl+D", nullptr, false);
		ImGui::MenuItem("Delete", "Supr", nullptr, false);

		ImGui::Separator();

		ImGui::MenuItem("Play", "Ctrl+P", nullptr, false);
		ImGui::MenuItem("Pause", "Ctrl+Shift+P", nullptr, false);
		ImGui::MenuItem("Step", "Ctrl+Alt+P", nullptr, false);

		ImGui::Separator();

		ImGui::MenuItem("Settings");

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
		ImGui::MenuItem("GuiDemo", "8", &App->editor->show_imgui_demo);
		ImGui::MenuItem("About", "9", &App->editor->show_about_popup);

		ImGui::EndMenu();
	}

	return ret;
}

bool E_Toolbar::ViewMainMenuItem()
{
	bool ret = true;

	if (ImGui::BeginMenu("View"))
	{
		bool show_grid = App->editor->GetShowWorldGrid();
		ImGui::MenuItem("World Grid", "F1", &show_grid);
		App->editor->SetShowWorldGrid(show_grid);
		
		bool show_axis = App->editor->GetShowWorldAxis();
		ImGui::MenuItem("World Axis", "F2", &show_axis);
		App->editor->SetShowWorldAxis(show_axis);

		ImGui::EndMenu();
	}

	return ret;
}

bool E_Toolbar::GameObjectsMainMenuItem()
{
	bool ret = true;

	if (ImGui::BeginMenu("GameObject"))
	{
		ImGui::MenuItem("Create Empty");
		ImGui::MenuItem("Create Empty x10");
		
		if (ImGui::BeginMenu("Primitives"))
		{
			bool show_primitive_examples = App->editor->GetShowPrimitiveExamples();
			ImGui::MenuItem("Show Examples", "", &show_primitive_examples);
			App->editor->SetShowPrimitiveExamples(show_primitive_examples);

			ImGui::Separator();

			ImGui::MenuItem("Cube", "", nullptr, false);
			ImGui::MenuItem("Sphere", "", nullptr, false);
			ImGui::MenuItem("Cylinder", "", nullptr, false);
			ImGui::MenuItem("Pyramid", "", nullptr, false);

			ImGui::EndMenu();
		}

		ImGui::MenuItem("Camera");

		ImGui::Separator();

		if (ImGui::BeginMenu("Draw Mode", false))
		{
			bool mesh = true;
			bool wire = false;
			bool vert = false;

			ImGui::MenuItem("Mesh", "TBD", &mesh);
			ImGui::MenuItem("Wireframe", "F3", &wire);
			ImGui::MenuItem("Vertices", "TBD", &vert);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Show", false))
		{
			bool norm = false;

			ImGui::MenuItem("Show Normals", "F4", &norm);

			ImGui::EndMenu();
		}

		ImGui::End();
	}

	return ret;
}

bool E_Toolbar::HelpMainMenuItem()
{
	bool ret = true;

	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::Button("About NULL Engine"))
		{
			App->editor->show_about_popup = true;
		}
		
		ImGui::Separator();
		
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
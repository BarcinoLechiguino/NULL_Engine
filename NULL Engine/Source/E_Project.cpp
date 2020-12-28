#include <vector>
#include <algorithm>

#include "Macros.h"

#include "Application.h" 
#include "M_FileSystem.h"
#include "M_Editor.h"

#include "E_Project.h"

#include "MemoryManager.h"

#define MAX_DIRECTORY_SIZE 500

E_Project::E_Project() : EditorPanel("Project"),
directory_to_display	(nullptr),
icons_are_loaded		(false)
{
	directory_to_display = new char[MAX_DIRECTORY_SIZE];

	sprintf_s(directory_to_display, MAX_DIRECTORY_SIZE, "%s", ASSETS_PATH);
}

E_Project::~E_Project()
{
	delete[] directory_to_display;
}

bool E_Project::Draw(ImGuiIO& io)
{
	bool ret = true;

	if (!icons_are_loaded)
	{
		App->editor->GetEngineIconsThroughEditor(engine_icons);
		icons_are_loaded = true;
	}

	ImGui::Begin("Project##", (bool*)0, ImGuiWindowFlags_MenuBar);

	GenerateDockspace(io);
	
	DrawMenuBar();
	DrawAssetsTree();
	DrawFolderExplorer();

	ImGui::End();

	return ret;
}

bool E_Project::CleanUp()
{
	bool ret = true;



	return ret;
}

// --- E_PRROJECT METHODS ---
void E_Project::GenerateDockspace(ImGuiIO& io) const
{
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("Project##");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	}
}

void E_Project::DrawMenuBar() const
{
	ImGui::BeginMenuBar();

	if (ImGui::BeginMenu("Options"))
	{
		ImGui::MenuItem("Show In Explorer", nullptr, false, false);

		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
}

void E_Project::DrawAssetsTree()
{
	ImGui::Begin("AssetsTree", false);

	if (ImGui::TreeNodeEx(ASSETS_PATH, ImGuiTreeNodeFlags_DefaultOpen))
	{
		DrawDirectoriesTree(ASSETS_PATH, DOTLESS_META_EXTENSION);
		ImGui::TreePop();
	}

	ImGui::End();
}

void E_Project::DrawFolderExplorer() const
{
	ImGui::Begin("FolderExplorer", false);

	ImGui::Text("WORK IN PROGRESS");

	ImGui::Image((ImTextureID)engine_icons.GetAnimationIconID(),	ImVec2(60, 60),		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::SameLine();
	ImGui::Image((ImTextureID)engine_icons.GetFileIconID(),			ImVec2(60, 60),		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::SameLine();
	ImGui::Image((ImTextureID)engine_icons.GetFolderIconID(),		ImVec2(60, 60),		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::SameLine();
	ImGui::Image((ImTextureID)engine_icons.GetMaterialIconID(),		ImVec2(60, 60),		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::SameLine();
	ImGui::Image((ImTextureID)engine_icons.GetModelIconID(),		ImVec2(60, 60),		ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}

void E_Project::DrawDirectoriesTree(const char* root_directory, const char* extension_to_filter)
{
	std::vector<std::string> directories;
	std::vector<std::string> files;

	std::string root_dir = root_directory;

	App->file_system->DiscoverFiles(root_dir.c_str(), files, directories);

	for (uint i = 0; i < directories.size(); ++i)
	{
		std::string path = root_dir + directories[i] + ("/");

		if (ImGui::TreeNodeEx(path.c_str(), 0, "%s/", directories[i].c_str()))
		{
			DrawDirectoriesTree(path.c_str(), extension_to_filter);
			ImGui::TreePop();
		}
	}

	std::sort(files.begin(), files.end());

	for (uint i = 0; i < files.size(); ++i)
	{
		if (App->file_system->GetFileExtension(files[i].c_str()) == extension_to_filter)
		{
			continue;
		}
		
		if (ImGui::TreeNodeEx(files[i].c_str(), ImGuiTreeNodeFlags_Leaf))
		{
			if (ImGui::IsItemClicked())
			{
				sprintf_s(directory_to_display, MAX_DIRECTORY_SIZE, "%s%s", root_dir.c_str(), files[i].c_str());
			}

			ImGui::TreePop();
		}
	}

	directories.clear();
	files.clear();
}
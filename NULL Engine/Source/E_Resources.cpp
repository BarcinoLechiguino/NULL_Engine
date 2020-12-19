#include <map>

#include "VariableTypedefs.h"

#include "Application.h"
#include "M_Editor.h"

#include "Resource.h"

#include "E_Resources.h"

E_Resources::E_Resources() : EditorPanel("Resources")
{

}

E_Resources::~E_Resources()
{

}

bool E_Resources::Draw(ImGuiIO& io)
{
	bool ret = true;

	ImGui::Begin("References");
	
	std::map<uint32, Resource*> resources;
	App->editor->GetResourcesThroughEditor(resources);

	std::map<uint32, Resource*>::iterator item;
	for (item = resources.begin(); item != resources.end(); ++item)
	{
		if (item->second == nullptr)
		{
			continue;
		}

		//ImGui::Text("Name:");		ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "      %s",		item->second->GetName());
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "%s", item->second->GetAssetsFile());
		
		ImGui::Text("UID:");		ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "       %lu",	item->second->GetUID());
		ImGui::Text("Type:");		ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "      %s",		item->second->GetTypeAsString());
		ImGui::Text("References:");	ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%u",			item->second->GetReferences());

		ImGui::Separator();
	}

	ImGui::End();

	return ret;
}

bool E_Resources::CleanUp()
{
	bool ret = true;



	return ret;
}
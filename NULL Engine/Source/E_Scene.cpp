#include "Application.h"
#include "M_Renderer3D.h"
#include "M_Window.h"

#include "E_Scene.h"

E_Scene::E_Scene() : EditorPanel("Scene")
{

}

E_Scene::~E_Scene()
{

}

bool E_Scene::Draw(ImGuiIO& io)
{
	bool ret = true;

	ImGui::Begin("Scene");

	CheckSceneIsClicked();

	float aspect_ratio = App->window->GetWidth() / App->window->GetHeight();

	ImVec2 tex_size = (ImGui::GetWindowSize() * aspect_ratio) * 0.925f;

	ImGui::SetCursorPos((ImGui::GetWindowSize() - tex_size) * 0.5f);
	ImGui::Image((ImTextureID)App->renderer->GetSceneRenderTexture(), tex_size, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
	//ImGui::Image((ImTextureID)App->renderer->GetDepthBufferTexture(), tex_size, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

	ImGui::End();

	return ret;
}

bool E_Scene::CleanUp()
{
	bool ret = true;



	return ret;
}

// --- E_SCENE METHODS ---
void E_Scene::CheckSceneIsClicked()
{
	if (ImGui::IsWindowHovered())
	{
		ImGui::FocusWindow(ImGui::GetCurrentWindow());
	}
	else
	{
		if (ImGui::IsWindowFocused())
		{
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
			{
				ImGui::FocusWindow(nullptr);
			}
		}
	}

	if (ImGui::IsWindowFocused())
	{
		SetIsClicked(true);
	}
	else
	{
		SetIsClicked(false);
	}
}
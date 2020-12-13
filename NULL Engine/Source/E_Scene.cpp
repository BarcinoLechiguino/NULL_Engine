#include "Application.h"
#include "M_Window.h"
#include "M_Renderer3D.h"
#include "M_Input.h"

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

	AdaptTextureToWindowSize();
	DrawSceneTexture();

	ImGui::End();

	return ret;
}

bool E_Scene::CleanUp()
{
	bool ret = true;



	return ret;
}

// --- E_SCENE METHODS ---
float2 E_Scene::GetSceneTextureSize()
{
	return float2(tex_size.x, tex_size.y);
}

float2 E_Scene::GetScreenMouseMotion()
{
	float2 win_mouse_motion	= float2((float)App->input->GetMouseXMotion(), (float)App->input->GetMouseYMotion());
	float2 win_size			= float2((float)App->window->GetWidth(), (float)App->window->GetHeight());
	float2 tex_size			= float2(this->tex_size.x, this->tex_size.y);

	float2 local_motion			= float2(win_mouse_motion.x / tex_size.x, win_mouse_motion.y / tex_size.y);
	float2 screen_mouse_motion	= float2(local_motion.x * win_size.x, local_motion.y * win_size.y);

	return screen_mouse_motion;
}

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
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) || ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
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

void E_Scene::AdaptTextureToWindowSize()
{	
	tex_size			= ImVec2((float)App->window->GetWidth(), (float)App->window->GetHeight());
	ImVec2 win_size		= ImGui::GetWindowSize() * 0.975f;													// An offset is added so the image is entirely enclosed by the window.

	float width_ratio	= (tex_size.x / win_size.x);														// tex.x to win.x ratio and tex.y to win.y ratio.
	float height_ratio	= (tex_size.y / win_size.y);														// Will be used to maintain the aspect ratio of the tex after win is resized.

	if (tex_size.x > win_size.x)
	{
		tex_size = tex_size / width_ratio;																	// Will adapt the tex to the width ratio between the tex and the win widths.
	}

	if (tex_size.y > win_size.y)
	{
		tex_size = tex_size / height_ratio;																	// Will adapt the tex to the height ratio between the tex and the win heights.
	}
}

void E_Scene::DrawSceneTexture()
{
	ImGui::SetCursorPos((ImGui::GetWindowSize() - tex_size) * 0.5f);
	ImGui::Image((ImTextureID)App->renderer->GetSceneRenderTexture(), tex_size, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
	//ImGui::Image((ImTextureID)App->renderer->GetDepthBufferTexture(), tex_size, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
}
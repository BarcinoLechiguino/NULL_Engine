#include "MathGeoTransform.h"
#include "MathGeoFrustum.h"

#include "Application.h"
#include "M_Window.h"
#include "M_Renderer3D.h"
#include "M_Input.h"
#include "M_Editor.h"

#include "GameObject.h"
#include "C_Transform.h"
#include "C_Camera.h"

#include "E_Scene.h"

E_Scene::E_Scene() : EditorPanel("Scene"),
tex_size			({ 0.0f, 0.0f }),
tex_origin			({ 0.0f, 0.0f }),
cursor_pos			({ 0.0f, 0.0f }),
guizmo_operation	(ImGuizmo::OPERATION::TRANSLATE),
guizmo_mode			(ImGuizmo::MODE::WORLD)
{

}

E_Scene::~E_Scene()
{

}

bool E_Scene::Draw(ImGuiIO& io)
{
	bool ret = true;

	ImGui::Begin("Game");
	
	CheckSceneIsClicked();
	
	AdaptTextureToWindowSize();
	DrawSceneTexture();
	
	ImGui::End();

	ImGui::Begin("Scene");

	SetIsHovered();

	CheckSceneIsClicked();

	AdaptTextureToWindowSize();
	DrawSceneTexture();

	HandleGuizmos();

	ImGui::End();

	return ret;
}

bool E_Scene::CleanUp()
{
	bool ret = true;



	return ret;
}

// --- E_SCENE METHODS ---
float2 E_Scene::GetWorldMousePosition()
{	
	float win_width		= (float)App->window->GetWidth();
	float win_height	= (float)App->window->GetHeight();
	
	float tex_width		= tex_size.x;
	float tex_height	= tex_size.y;

	float mouse_X		= (float)App->input->GetMouseX();
	float mouse_Y		= (float)App->input->GetMouseY();

	float2 screen_mouse_pos = float2(mouse_X, win_height - mouse_Y) - float2(tex_origin.x, tex_origin.y + 22.5f);				// TMP. Un-Hardcode Later.
	//float2 screen_mouse_pos = GetScreenMousePosition();
	float2 norm_screen_pos	= float2(screen_mouse_pos.x / tex_width, screen_mouse_pos.y / tex_height);
	float2 world_mouse_pos	= float2(norm_screen_pos.x * win_width, norm_screen_pos.y * win_height);

	return world_mouse_pos;
}

float2 E_Scene::GetScreenMousePosition()
{
	float win_width		= (float)App->window->GetWidth();
	float win_height	= (float)App->window->GetHeight();

	float tex_width		= tex_size.x;
	float tex_height	= tex_size.y;

	float mouse_X		= (float)App->input->GetMouseX();
	float mouse_Y		= (float)App->input->GetMouseY();

	float2 world_mouse_pos	= float2(mouse_X, mouse_Y);
	float2 norm_world_pos	= float2(world_mouse_pos.x / win_width, world_mouse_pos.y / win_height);
	float2 screen_mouse_pos = float2(norm_world_pos.x * tex_width, norm_world_pos.y * tex_height);

	screen_mouse_pos		+= float2(tex_origin.x, tex_origin.y);

	return screen_mouse_pos;
}

float2 E_Scene::GetWorldMouseMotion()
{
	float2 win_mouse_motion	= float2((float)App->input->GetMouseXMotion(), (float)App->input->GetMouseYMotion());
	float2 win_size			= float2((float)App->window->GetWidth(), (float)App->window->GetHeight());
	float2 tex_size			= float2(this->tex_size.x, this->tex_size.y);

	float2 local_motion			= float2(win_mouse_motion.x / tex_size.x, win_mouse_motion.y / tex_size.y);
	float2 world_mouse_motion	= float2(local_motion.x * win_size.x, local_motion.y * win_size.y);

	return world_mouse_motion;
}

float2 E_Scene::GetSceneTextureSize()
{
	return float2(tex_size.x, tex_size.y);
}

bool E_Scene::UsingGuizmo()
{
	return ImGuizmo::IsUsing();
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
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) 
				|| ImGui::IsMouseClicked(ImGuiMouseButton_Right) 
				|| ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
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
	cursor_pos = (ImGui::GetWindowSize() - tex_size) * 0.5f;
	ImGui::SetCursorPos(cursor_pos);

	ImVec2 screen_cursor_pos = ImGui::GetCursorScreenPos();

	if (screen_cursor_pos.x > 1920)																			// Need to take into account having multiple screens. Un-Harcode Later!
	{
		screen_cursor_pos.x = screen_cursor_pos.x - 1920;
	}

	tex_origin		= screen_cursor_pos + ImVec2(0, tex_size.y);											// Getting the top-left corner at XY.
	tex_origin.y	= (float)App->window->GetHeight() - tex_origin.y;										// Converting from top-left Y origin to bottom-left Y origin.

	ImGui::Image((ImTextureID)App->renderer->GetSceneRenderTexture(), tex_size, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

	//ImGui::Image((ImTextureID)App->renderer->GetDepthBufferTexture(), tex_size, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
}

void E_Scene::HandleGuizmos()
{	
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_DOWN)
	{
		guizmo_operation	= ImGuizmo::OPERATION::TRANSLATE;
		guizmo_mode			= ImGuizmo::MODE::WORLD;
	}
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_STATE::KEY_DOWN)
	{
		guizmo_operation	= ImGuizmo::OPERATION::ROTATE;
		guizmo_mode			= ImGuizmo::MODE::WORLD;
	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_STATE::KEY_DOWN)
	{
		guizmo_operation	= ImGuizmo::OPERATION::SCALE;
		guizmo_mode			= ImGuizmo::MODE::LOCAL;
	}
	
	if (App->editor->GetSelectedGameObjectThroughEditor() == nullptr)
	{
		return;
	}

	if (App->editor->GetCurrentCameraThroughEditor() == nullptr)
	{
		return;
	}
	
	GameObject* selected		= App->editor->GetSelectedGameObjectThroughEditor();
	C_Camera* current_camera	= App->editor->GetCurrentCameraThroughEditor();

	float4x4 view_matrix		= current_camera->GetFrustum().ViewMatrix();
	float4x4 projection_matrix	= current_camera->GetFrustum().ProjectionMatrix();
	float4x4 world_transform	= selected->GetTransformComponent()->GetWorldTransform();
	view_matrix.Transpose();																				// MathGeoLib works with Row-Major matrices and ImGuizmo works with
	projection_matrix.Transpose();																			// Column-Major matrices. Hence the need to transpose them.
	world_transform.Transpose();																			// ----------------------------------------------------------------

	ImGuizmo::SetDrawlist();

	float win_height	= (float)App->window->GetHeight();
	float tex_width		= tex_size.y;
	ImVec2 origin_pos	= ImVec2(tex_origin.x, win_height - tex_origin.y - tex_width);

	ImGuizmo::SetRect(tex_origin.x, origin_pos.y, tex_size.x, tex_size.y);

	ImGuizmo::Manipulate(view_matrix.ptr(), projection_matrix.ptr(), guizmo_operation, guizmo_mode, world_transform.ptr());

	if (ImGuizmo::IsUsing())
	{
		world_transform = world_transform.Transposed();
		selected->GetTransformComponent()->SetWorldTransform(world_transform);
	}
}

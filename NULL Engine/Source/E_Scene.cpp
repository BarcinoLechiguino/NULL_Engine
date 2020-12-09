#include "Application.h"
#include "M_Renderer3D.h"

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

	ImVec2 win_size = ImGui::GetWindowSize();
	ImGui::Image((ImTextureID)App->renderer->GetSceneRenderTexture(), win_size, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

	ImGui::End();

	return ret;
}

bool E_Scene::CleanUp()
{
	bool ret = true;



	return ret;
}
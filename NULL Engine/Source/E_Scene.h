#ifndef __E_SCENE_H__
#define __E_SCENE_H__

#include "MathGeoLib/include/Math/float2.h"
#include "ImGuizmo/include/ImGuizmo.h"
#include "EditorPanel.h"

class E_Scene : public EditorPanel
{
public:
	E_Scene();
	~E_Scene();

	bool Draw			(ImGuiIO& io) override;

	bool CleanUp		() override;

private:
	void CheckSceneIsClicked		();

	void AdaptTextureToWindowSize	();
	void DrawSceneTexture			();

	void HandleGuizmos				();

public:
	float2 GetWorldMousePosition();
	float2 GetScreenMousePosition();
	float2 GetWorldMouseMotion();
	float2 GetSceneTextureSize();

	bool UsingGuizmo();

private:
	ImVec2					tex_size;
	ImVec2					tex_origin;
	ImVec2					cursor_pos;

	ImGuizmo::OPERATION		guizmo_operation;
	ImGuizmo::MODE			guizmo_mode;
};

#endif // !__E_SCENE_H__
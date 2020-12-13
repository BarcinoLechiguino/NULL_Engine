#ifndef __E_SCENE_H__
#define __E_SCENE_H__

#include "MathGeoLib/include/Math/float2.h"
#include "EditorPanel.h"

class E_Scene : public EditorPanel
{
public:
	E_Scene();
	~E_Scene();

	bool Draw(ImGuiIO& io) override;

	bool CleanUp() override;

public:
	float2 GetWorldMousePosition();
	float2 GetWorldMouseMotion();
	float2 GetSceneTextureSize();

private:
	void CheckSceneIsClicked();

	void AdaptTextureToWindowSize();
	void DrawSceneTexture();

private:
	ImVec2	tex_size;
};

#endif // !__E_SCENE_H__
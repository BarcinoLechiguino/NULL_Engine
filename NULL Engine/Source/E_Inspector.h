#ifndef __E_INSPECTOR_H__
#define __E_INSPECTOR_H__

#include "E_Panel.h"

class GameObject;

class E_Inspector : public E_Panel
{
public:
	E_Inspector();
	~E_Inspector();

	bool Draw(ImGuiIO& io) override;

	bool CleanUp() override;

private:
	void DrawGameObjectInfo(GameObject* selected_game_object);
	void DrawComponents(GameObject* selected_game_object);
	void DrawTransformComponent(GameObject* selected_game_object);
	void DrawMeshComponent(GameObject* selected_game_object);
	void DrawMaterialComponent(GameObject* selected_game_object);
	void DrawLightComponent(GameObject* selected_game_object);
	void DrawCameraComponent(GameObject* selected_game_object);

	// ------- DRAW COMPONENT METHODS -------


private:
	//GameObject* selected_game_object;
};

#endif // !__E_INSPECTOR_H__
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

public:
	void SetSelectedGameObject(GameObject* game_object);
	GameObject* GetSelectedGameObject() const;

private:
	void DrawGameObjectInfo();
	void DrawComponents();
	void DrawTransformComponent();
	void DrawMeshComponent();
	void DrawMaterialComponent();
	void DrawLightComponent();

private:
	GameObject* selected_game_object;
};

#endif // !__E_INSPECTOR_H__
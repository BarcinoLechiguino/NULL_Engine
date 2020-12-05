#ifndef __E_INSPECTOR_H__
#define __E_INSPECTOR_H__

#include "E_Panel.h"

class GameObject;
class Component;
class C_Transform;
class C_Mesh;
class C_Material;
class C_Light;
class C_Camera;

class E_Inspector : public E_Panel
{
public:
	E_Inspector();
	~E_Inspector();

	bool Draw		(ImGuiIO& io) override;
	bool CleanUp	() override;

private:
	void DrawGameObjectInfo			(GameObject* selected_game_object);
	void DrawComponents				(GameObject* selected_game_object);

	void DrawTransformComponent		(C_Transform* c_transform);
	void DrawMeshComponent			(C_Mesh* c_mesh);
	void DrawMaterialComponent		(C_Material* c_material);
	void DrawLightComponent			(C_Light* c_light);
	void DrawCameraComponent		(C_Camera* c_camera);

	// ------- DRAW COMPONENT METHODS -------
	void AddComponentCombo			(GameObject* selected_game_object);					// 
	void DeleteComponentPopup		(GameObject* selected_game_object);					// 

	void DisplayTextureData			(C_Material* c_material);							// Will display the texture's width, height, depth...
	void TextureDisplay				(C_Material* c_material);							// Will display the texture as an image through Dear ImGui.

private:
	bool		show_delete_component_popup;
	int			component_type;

	int			map_to_display;

	Component*	component_to_delete;
};

#endif // !__E_INSPECTOR_H__
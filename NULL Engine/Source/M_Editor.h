#ifndef __M_EDITOR_H__
#define __M_EDITOR_H__

#include "ImGui/imgui.h"
#include "Module.h"

class M_Editor : public Module
{
public:
	M_Editor();
	~M_Editor();

	bool Init();
	bool Start();
	UPDATE_STATUS PreUpdate(float dt);
	UPDATE_STATUS Update(float dt);
	UPDATE_STATUS PostUpdate(float dt);
	bool CleanUp();

public:
	ImGuiIO io;
	bool show_demo_window;
	bool show_another_window;
	ImVec4 clear_color;

	float f;
	int counter;

private:
	int b;
};

#endif // !__M_EDITOR_H__
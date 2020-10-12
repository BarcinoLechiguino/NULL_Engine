#ifndef __M_EDITOR_H__
#define __M_EDITOR_H__

#include "ImGui/imgui.h"
#include "Module.h"

class Configuration;

enum class IMGUI_STYLE
{
	CLASSIC,
	LIGHT,
	DARK,
	OTHER
};

class M_Editor : public Module
{
public:
	M_Editor(bool is_active = true);
	~M_Editor();

	bool Init(Configuration& config) override;
	bool Start() override;
	UPDATE_STATUS PreUpdate(float dt) override;
	UPDATE_STATUS Update(float dt) override;
	UPDATE_STATUS PostUpdate(float dt) override;
	bool CleanUp() override;

public:
	ImGuiIO io;
	bool show_demo_window;
	bool show_another_window;
	ImVec4 clear_color;

	int current_style;
	const char* styles;

	float f;
	int counter;

private:
	int b;
};

#endif // !__M_EDITOR_H__
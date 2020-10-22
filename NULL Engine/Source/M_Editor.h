#ifndef __M_EDITOR_H__
#define __M_EDITOR_H__

#include "ImGui/imgui.h"
#include "Module.h"

class Configuration;

class E_Panel;
class E_Test;
class E_Toolbar;
class E_About;
class E_Console;

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

	bool			Init(Configuration& config) override;
	bool			Start() override;
	UPDATE_STATUS	PreUpdate(float dt) override;
	UPDATE_STATUS	Update(float dt) override;
	UPDATE_STATUS	PostUpdate(float dt) override;
	bool			CleanUp() override;

	bool LoadConfiguration(Configuration& root) override;
	bool SaveConfiguration(Configuration& root) const override;

public:
	bool GetEvent(SDL_Event* event) const;											// Will return false if there was no event to read.

	void AddGuiPanel(E_Panel* panel);												//
	void AddConsoleLog(const char* log);

	bool RenderGuiPanels() const;													//
	bool InitializeImGui() const;

public:
	std::vector<E_Panel*>	gui_panels;												//

	E_Test*					test;													//
	E_Toolbar*				toolbar;
	E_About*				about;
	E_Console*				console;

public:
	ImVec4					clear_color;											//

private:
	int b;
};

#endif // !__M_EDITOR_H__
#ifndef __M_EDITOR_H__
#define __M_EDITOR_H__

#include "ImGui/imgui.h"
#include "Module.h"

class Configuration;

class E_Panel;
class E_Toolbar;
class E_EngineConfiguration;
class E_Hierarchy;
class E_Inspector;
class E_Console;
class E_ImGuiDemo;
class E_About;

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

	bool			LoadConfiguration(Configuration& root) override;
	bool			SaveConfiguration(Configuration& root) const override;

public:
	bool			GetEvent(SDL_Event* event) const;								// Will return false if there was no event to read.

	void			AddGuiPanel(E_Panel* panel);									//

	void			EditorShortcuts();
	void			CheckShowHideFlags();

	bool			RenderGuiPanels() const;										//
	bool			InitializeImGui() const;

public:																				// --- Panel/Window Methods. Acts as an interface between other modules and the panels. Avoids dependencies.
	bool			GetShowWorldGrid() const;
	bool			GetShowWorldAxis() const;
	bool			GetShowPrimitiveExamples() const;
	void			SetShowWorldGrid(bool set_to);
	void			SetShowWorldAxis(bool set_to);
	void			SetShowPrimitiveExamples(bool set_to);

	void			UpdateFrameData(int frames, int ms);

	void			AddConsoleLog(const char* log);
	void			AddInputLog(uint key, uint state);

private:
	bool BeginRootWindow(ImGuiIO& io, const char* window_id, bool docking, ImGuiWindowFlags window_flags = ImGuiWindowFlags_None);
	void BeginDockspace(ImGuiIO& io, const char* dockspace_id, ImGuiDockNodeFlags docking_flags = ImGuiDockNodeFlags_None, ImVec2 size = { 0.0f, 0.0f });

public:
	std::vector<E_Panel*>	gui_panels;												//

	E_Toolbar*				toolbar;												// 
	E_EngineConfiguration*	configuration;											// 
	E_Hierarchy*			hierarchy;												// 
	E_Inspector*			inspector;												// 
	E_Console*				console;												// 
	E_ImGuiDemo*			imgui_demo;												// 
	E_About*				about;													// 

public:
	ImVec4 clear_color;																//

	bool show_configuration;														// 
	bool show_hierarchy;															// 
	bool show_inspector;															// 
	bool show_console;																// 
	bool show_imgui_demo;															// 
	bool show_about_popup;															// 
	bool show_close_app_popup;														// 

private:
	int b;
};

#endif // !__M_EDITOR_H__
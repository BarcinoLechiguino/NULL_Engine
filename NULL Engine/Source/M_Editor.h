#ifndef __M_EDITOR_H__
#define __M_EDITOR_H__

#include "ImGui/include/imgui.h"
#include "MathGeoLib/include/Math/float2.h"
#include "Module.h"

class ParsonNode;

class GameObject;
class C_Camera;

class EditorPanel;
class E_MainMenuBar;
class E_Toolbar;
class E_Configuration;
class E_Hierarchy;
class E_Inspector;
class E_Console;
class E_Project;
class E_Scene;
class E_Game;
class E_ImGuiDemo;
class E_About;
class E_LoadFile;

enum class IMGUI_STYLE
{
	CLASSIC,
	LIGHT,
	DARK,
	OTHER
};

enum class EDITOR_EVENT
{
	CREATE_GAMEOBJECT,
	SELECT_GAMEOBJECT,
	DELETE_GAME_OBJECT,
	LOAD_FILE
};

class M_Editor : public Module
{
public:
	M_Editor(bool is_active = true);
	~M_Editor();

	bool			Init				(ParsonNode& config) override;
	bool			Start				() override;
	UPDATE_STATUS	PreUpdate			(float dt) override;
	UPDATE_STATUS	Update				(float dt) override;
	UPDATE_STATUS	PostUpdate			(float dt) override;
	bool			CleanUp				() override;

	bool			LoadConfiguration	(ParsonNode& root) override;
	bool			SaveConfiguration	(ParsonNode& root) const override;

public:
	bool			GetEvent							(SDL_Event* event) const;				// Will return false if there was no event to read.

	void			AddEditorPanel						(EditorPanel* panel);					// Will add the E_Panel* passed as argument to the gui_panels vector.

	void			EditorShortcuts						();										// All the shortcuts related to the editor have been gathered here.
	void			CheckShowHideFlags					();										// Will check whether or not each of the panels must be enabled or disabled.

	bool			EditorIsBeingHovered				() const;								// Will check whether or not any of the editor panels is being hovered.
	bool			EditorSceneIsBeingClicked			() const;
	bool			RenderEditorPanels					() const;								// Will call ImGui::Render() to render all the panels on the screen.
	bool			InitializeImGui						() const;								// Creates an ImGui Context and sets an initial configuration for it.


public:																							// --- Panel/Window Methods. Acts as an interface between other modules and the panels.
	bool			GetShowWorldGrid					() const;								// Will return true if the World Grid is being currently shown.
	bool			GetShowWorldAxis					() const;								// Will return true if the World Axis is being currently shown.
	bool			GetShowPrimitiveExamples			() const;								// Will return true if the Primitive Examples are being currently shown.
	void			SetShowWorldGrid					(bool set_to);							// Will Enable or Disable the World Grid depending on the passed argument.
	void			SetShowWorldAxis					(bool set_to);							// Will Enable or Disable the depending on the passed argument.
	void			SetShowPrimitiveExamples			(bool set_to);							// Will Enable or Disable the depending on the passed argument.

	void			UpdateFrameData						(int frames, int ms);					// Configuration: Passing the received frame data to the configuration editor module.
	void			AddInputLog							(uint key, uint state);					// Configuration: Receives an input key and a state and passes a log to the config editor mod.

	void			AddConsoleLog						(const char* log);						// Console: Passing the received console log to the console editor module.

	GameObject*		GetSceneRootThroughEditor			() const;								// Hierarchy & inspector: Will return the current root GameObjcect.
	void			SetSceneRootThroughEditor			(GameObject* game_object);				// Hierarchy & inspector: Will set the scene's root GameObject with the passed one.
	GameObject*		GetSelectedGameObjectThroughEditor	() const;								// Hierarchy & Inspector: Will return the currently selected GameObject.
	void			SetSelectedGameObjectThroughEditor	(GameObject* game_object);				// Hierarchy & Inspector: Will set the scene's selected GameObject with the passed one.
	void			DeleteSelectedGameObject			();										// Hierarchy & Inspector: Will call the Module Scene's delete selected GameObject method.
	bool			SelectedIsSceneRoot					() const;								// Hierarchy & Inspector: Will check whether or not the selected GameObject is the scene root.
	void			CreateGameObject					(const char* name, GameObject* parent);	// Hierarchy & Inspector: Will call the Module Scene's create GameObject method.
	C_Camera*		GetCurrentCameraThroughEditor		() const;								// Hierarchy & Inspector: 
	void			SetCurrentCameraThroughEditor		(C_Camera* game_object);				// Hierarchy & Inspector:  
	void			SetMasterCameraThroughEditor		();										// Hierarchy & Inspector: 

	float2			GetWorldMousePositionThroughEditor	() const;
	float2			GetWorldMouseMotionThroughEditor	() const;
	float2			GetSceneTextureSizeThroughEditor	() const;

	bool			SceneIsHovered						() const;
	bool			UsingGuizmoInScene					() const;

	void			LoadFileThroughEditor				(const char* path);						// Load File: Will send the given path to the Importer.

private:
	bool BeginRootWindow(ImGuiIO& io, const char* window_id, bool docking, ImGuiWindowFlags window_flags = ImGuiWindowFlags_None);							// Generates a root window for docking.
	void BeginDockspace(ImGuiIO& io, const char* dockspace_id, ImGuiDockNodeFlags docking_flags = ImGuiDockNodeFlags_None, ImVec2 size = { 0.0f, 0.0f });	// Generates a new dockspace.

public:
	std::vector<EditorPanel*>	editor_panels;													// Will store all the editor modules. Will be iterated for drawing all the panels.

	E_MainMenuBar*				main_menu_bar;
	E_Toolbar*					toolbar;
	E_Configuration*			configuration;
	E_Hierarchy*				hierarchy;
	E_Inspector*				inspector;
	E_Console*					console;
	E_Project*					project;
	E_Scene*					scene;
	E_Game*						game;
	E_ImGuiDemo*				imgui_demo;
	E_About*					about;
	E_LoadFile*					load_file;

	ImVec4						clear_color;													// Will be used to set the clear color of the rendering environment.

	bool						show_configuration;												// Enable/Disable the Configuration window.
	bool						show_hierarchy;													// Enable/Disable the Hierarchy window.
	bool						show_inspector;													// Enable/Disable the Inspector window.
	bool						show_console;													// Enable/Disable the Console window.
	bool						show_project;													// Enable/Disable the Project window.
	bool						show_imgui_demo;												// Enable/Disable the ImGui Demo window.
	bool						show_about_popup;												// Enable/Disable the About window popup.
	bool						show_load_file_popup;											// Enable/Disable the Load File popup.
	bool						show_close_app_popup;											// Enable/Disable the Close App popup.
};

#endif // !__M_EDITOR_H__
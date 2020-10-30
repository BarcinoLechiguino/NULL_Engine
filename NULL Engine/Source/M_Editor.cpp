#include "ImGui.h"
#include "OpenGL.h"
#include "ImGui/imgui_internal.h"

#include "Application.h"
#include "M_Window.h"
#include "M_Renderer3D.h"
#include "M_Input.h"

#include "E_Panel.h"
#include "E_Toolbar.h"
#include "E_EngineConfiguration.h"
#include "E_Hierarchy.h"
#include "E_Inspector.h"
#include "E_Console.h"
#include "E_ImGuiDemo.h"
#include "E_About.h"

#include "M_Editor.h"

#pragma comment (lib, "Source/Dependencies/glew/libx86/glew32.lib")

M_Editor::M_Editor(bool is_active) : Module("Editor", is_active),
clear_color(0.0f, 0.0f, 0.0f, 1.0f),
toolbar(nullptr),
configuration(nullptr),
hierarchy(nullptr),
inspector(nullptr),
console(nullptr),
imgui_demo(nullptr),
about(nullptr)
{
	toolbar			= new E_Toolbar();
	configuration	= new E_EngineConfiguration();
	hierarchy		= new E_Hierarchy();
	inspector		= new E_Inspector();
	console			= new E_Console();
	imgui_demo		= new E_ImGuiDemo();
	about			= new E_About();

	AddGuiPanel(toolbar);
	AddGuiPanel(configuration);
	AddGuiPanel(hierarchy);
	AddGuiPanel(inspector);
	AddGuiPanel(console);
	AddGuiPanel(imgui_demo);
	AddGuiPanel(about);

	show_configuration		= true;
	show_hierarchy			= true;
	show_inspector			= true;
	show_console			= true;
	show_imgui_demo			= false;
	show_about_popup		= false;
	show_close_app_popup	= false;
}

M_Editor::~M_Editor()
{

}

bool M_Editor::Init(Configuration& config)
{
	bool ret = true;

	return ret;
}

bool M_Editor::Start()
{
	bool ret = true;

	InitializeImGui();
	
	return ret;
}

UPDATE_STATUS M_Editor::PreUpdate(float dt)
{
	UPDATE_STATUS ret = UPDATE_STATUS::CONTINUE;

	EditorShortcuts();
	CheckShowHideFlags();

	return ret;
}

UPDATE_STATUS M_Editor::Update(float dt)
{
	UPDATE_STATUS ret = UPDATE_STATUS::CONTINUE;

	return ret;
}

UPDATE_STATUS M_Editor::PostUpdate(float dt)
{
	UPDATE_STATUS ret = UPDATE_STATUS::CONTINUE;
	
	ImGuiIO& io = ImGui::GetIO();

	//Start Dear ImGui's frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->GetWindow());
	ImGui::NewFrame();

	if (BeginRootWindow(io, "Root window", true, ImGuiWindowFlags_MenuBar))
	{
		bool draw = true;
		for (uint i = 0; i < gui_panels.size(); ++i)
		{
			if (gui_panels[i]->IsActive())
			{
				draw = gui_panels[i]->Draw(io);

				if (!draw)
				{
					ret = UPDATE_STATUS::STOP;
					LOG("[EDITOR] Exited through %s Panel", gui_panels[i]->GetName());
					break;
				}
			}
		}
		
		ImGui::End();
	}
	
	return ret;
}

bool M_Editor::CleanUp()
{
	for (uint i = 0; i < gui_panels.size(); ++i)
	{
		gui_panels[i]->CleanUp();
		RELEASE(gui_panels[i]);
	}

	gui_panels.clear();
	
	// ImGui CleanUp()
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
	return true;
}

bool M_Editor::LoadConfiguration(Configuration& root)
{
	bool ret = true;

	return ret;
}

bool M_Editor::SaveConfiguration(Configuration& root) const
{
	bool ret = true;

	return ret;
}

// -------------- EDITOR METHODS --------------
bool M_Editor::GetEvent(SDL_Event* event) const
{
	return ImGui_ImplSDL2_ProcessEvent(event);											
}

void M_Editor::AddGuiPanel(E_Panel* panel)
{
	gui_panels.push_back(panel);
}

bool M_Editor::GetShowWorldGrid() const
{
	return App->renderer->GetDrawWorldGrid();
}

bool M_Editor::GetShowWorldAxis() const
{
	return App->renderer->GetDrawWorldAxis();
}

bool M_Editor::GetShowPrimitiveExamples() const
{
	return App->renderer->GetDrawPrimitiveExamples();
}

void M_Editor::SetShowWorldGrid(bool set_to)
{
	App->renderer->SetDrawWorldGrid(set_to);
}

void M_Editor::SetShowWorldAxis(bool set_to)
{
	App->renderer->SetDrawWorldAxis(set_to);
}

void M_Editor::SetShowPrimitiveExamples(bool set_to)
{
	App->renderer->SetDrawPrimtiveExamples(set_to);
}

void M_Editor::UpdateFrameData(int frames, int ms)
{
	configuration->UpdateFrameData(frames, ms);
}

void M_Editor::AddConsoleLog(const char* log)
{
	if (gui_panels.size() > 0)
	{
		if (console != nullptr)
		{
			console->AddLog(log);
		}
	}
}

void M_Editor::AddInputLog(uint key, uint state)
{
	char input[128];
	const char* states[] = { "IDLE", "DOWN", "REPEAT", "UP" };										// We add the 4 main key states. In practice "IDLE" will not be displayed.
	
	if (configuration != nullptr)
	{
		if (key < App->input->GetMaxNumScancodes())
		{
			const char* key_name = SDL_GetKeyName(SDL_GetKeyFromScancode((SDL_Scancode)key));			// Through the scancode it is possible to get a string with the name of the key.

			sprintf_s(input, 128, "[KEY] %02u %s - %s\n", key, key_name, states[state]);
		}
		else
		{
			uint mouse_button = key - App->input->GetMaxNumScancodes();

			sprintf_s(input, 128, "[MOUSE] %02u - %s\n", mouse_button, states[state]);
		}

		configuration->AddInputLog(input);
	}
}

void M_Editor::EditorShortcuts()
{	
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_STATE::KEY_DOWN)
	{
		show_configuration = !show_configuration;
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_STATE::KEY_DOWN)
	{
		show_hierarchy = !show_hierarchy;
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_STATE::KEY_DOWN)
	{
		show_inspector = !show_inspector;
	}

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_STATE::KEY_DOWN)
	{
		show_console = !show_console;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_8) == KEY_STATE::KEY_DOWN)
	{
		show_imgui_demo = !show_imgui_demo;
	}

	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_STATE::KEY_DOWN)
	{
		show_about_popup = !show_about_popup;
	}

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_STATE::KEY_DOWN)
	{
		show_close_app_popup = !show_close_app_popup;
	}
}

void M_Editor::CheckShowHideFlags()
{
	// --- ENGINE CONFIGRUATION ---
	if (show_configuration)
	{
		configuration->Enable();
	}
	else
	{
		configuration->Disable();
	}

	// --- HIERARCHY ---
	if (show_hierarchy)
	{
		hierarchy->Enable();
	}
	else
	{
		hierarchy->Disable();
	}

	// --- INSPECTOR ---
	if (show_inspector)
	{
		inspector->Enable();
	}
	else
	{
		inspector->Disable();
	}

	// --- CONSOLE ---
	if (show_console)
	{
		console->Enable();
	}
	else
	{
		console->Disable();
	}

	// --- IMGUI DEMO ---
	if (show_imgui_demo)
	{
		imgui_demo->Enable();
	}
	else
	{
		imgui_demo->Disable();
	}

	// --- ABOUT POPUP ---
	if (show_about_popup)
	{
		about->Enable();
	}
	else
	{
		about->Disable();
	}
}

bool M_Editor::BeginRootWindow(ImGuiIO& io, const char* window_id, bool docking, ImGuiWindowFlags window_flags)
{
	bool ret = true;
	
	ImGuiViewport* viewport = ImGui::GetWindowViewport();

	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
					| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus
					| ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ret = ImGui::Begin(window_id, &ret, window_flags);
	ImGui::PopStyleVar(3);

	if (docking)
	{	
		BeginDockspace(io, window_id, ImGuiDockNodeFlags_PassthruCentralNode);
	}

	return ret;
}	
	
void M_Editor::BeginDockspace(ImGuiIO& io, const char* dockspace_id, ImGuiDockNodeFlags docking_flags, ImVec2 size)
{
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dckspace_id = ImGui::GetID(dockspace_id);
		ImGui::DockSpace(dckspace_id, size, docking_flags);
	}
}

bool M_Editor::RenderGuiPanels() const
{
	// Rendering all ImGui elements
	ImGuiIO& io = ImGui::GetIO();
	
	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Updating and rendering additional platform windows
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{	
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();

		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();

		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}

	return true;
}

bool M_Editor::InitializeImGui() const
{
	bool ret = true;

	// Setting up Dear ImGui's context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();								// Needs to be called multiple times during a frame to update IO correctly.
	(void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		// Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;			// Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;			// Enable MultiViewport / Platform Windows

	//Setting up Dear ImGui's style.
	ImGui::StyleColorsDark();

	// Tweaking the platform windows to look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setting up Platfor/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->GetWindow(), App->renderer->context);
	ImGui_ImplOpenGL3_Init(0);

	return ret;
}
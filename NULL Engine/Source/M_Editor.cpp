#include "ImGui.h"
#include "OpenGL.h"
#include "ImGui/imgui_internal.h"

#include "Application.h"
#include "M_Window.h"
#include "M_Renderer3D.h"

#include "E_Panel.h"
#include "E_Test.h"
#include "E_Toolbar.h"
#include "E_About.h"
#include "E_EngineConfiguration.h"
#include "E_Console.h"

#include "M_Editor.h"

#pragma comment (lib, "Source/Dependencies/glew/libx86/glew32.lib")

M_Editor::M_Editor(bool is_active) : Module("Editor", is_active),
clear_color(0.0f, 0.0f, 0.0f, 1.0f),
test(nullptr),
toolbar(nullptr),
about(nullptr),
configuration(nullptr),
console(nullptr)
{
	test			= new E_Test();
	toolbar			= new E_Toolbar();
	about			= new E_About();
	configuration	= new E_EngineConfiguration();
	console			= new E_Console();

	AddGuiPanel(test);
	AddGuiPanel(toolbar);
	AddGuiPanel(about);
	AddGuiPanel(configuration);
	AddGuiPanel(console);
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
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	bool draw = true;
	for (int i = 0; i < gui_panels.size(); ++i)
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
	
	return ret;
}

bool M_Editor::CleanUp()
{
	for (int i = 0; i < gui_panels.size(); ++i)
	{
		gui_panels[i]->CleanUp();
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

void M_Editor::AddConsoleLog(const char* log)
{
	if (gui_panels.size() > 0)
	{
		console->AddLog(log);
	}
}

void M_Editor::UpdateFrameData(int frames, int ms)
{
	configuration->UpdateFrameData(frames, ms);
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
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;		// Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;			// Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;			// Enable MultiViewport / Platform Windows

	//Setting up Dear ImGui's style.
	ImGui::StyleColorsDark();
	/*ImGui::StyleColorsClassic();*/

	// Tweaking the platform windows to look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setting up Platfor/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init(0);

	return ret;
}
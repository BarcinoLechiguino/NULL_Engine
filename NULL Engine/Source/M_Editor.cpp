#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "OpenGL.h"
#include "ImGui.h"

#include "M_Editor.h"

#pragma comment (lib, "Source/Dependencies/glew/libx86/glew32.lib")

M_Editor::M_Editor() : Module("Editor", true)
{

}

M_Editor::~M_Editor()
{

}

bool M_Editor::Init()
{
	bool ret = true;
	
	return ret;
}

bool M_Editor::Start()
{
	bool ret = true;

	// ImGui ----------------------------------------------
	// Setting up Dear ImGui's context
	ImGui::CreateContext();
	io = ImGui::GetIO();
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
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGui_ImplOpenGL3_Init(0);

	// Initializing some variables
	show_demo_window = true;
	show_another_window = false;
	clear_color = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);

	f = 0.0f;
	counter = 0;
	// ----------------------------------------------------
	
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
	
	// ImGui -----------------------------------------------
	SDL_Event event;

	// Poll and handle events (input, window resize...)
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);

		if (event.type == SDL_QUIT)
		{
			return UPDATE_STATUS::STOP;
		}

		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(App->window->window))
		{
			return UPDATE_STATUS::STOP;
		}
	}

	//Start Dear ImGui's frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	// Showing ImGui's predetermined demo window
	if (show_demo_window)
	{
		ImGui::ShowDemoWindow();
	}

	// Creating a simple window
	{
		ImGui::Begin("Hello world!");								// Will create a window with "Hello World!" as the title. Until ImGui::End() all elements will be appended to this window.

		if (ImGui::Button("SHOW ME THE DATA"))
		{
			//return UPDATE_STATUS::STOP;

			App->display_framerate_data = !App->display_framerate_data;
		}

		ImGui::SameLine();

		if (ImGui::Button("YEETUS THIS WINDOW"))
		{
			return UPDATE_STATUS::STOP;
		}

		ImGui::Text("This text has been brought to you by Euro Shave Club.");	// Will create a label. Can also use format strings.
		ImGui::Checkbox("ImGui Demo Window", &show_demo_window);			// Checkbox that will modify the bool that it gets passed as argument.
		ImGui::Checkbox("Sneaky Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);				// Will create a slider that will be able to edit 1 variable from 0.0f to 1.0f.
		ImGui::ColorEdit3("clear color", (float*)&clear_color);		// Will create 3 sliders in a row that will represent a colour (RGB).

		if (ImGui::Button("Button"))
		{
			++counter;
		}

		//ImGui::PlotHistogram();

		ImGui::SameLine();											// Specifies that the next element to be created will be created in the same row as the previous one.
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	if (show_another_window)
	{
		ImGui::Begin("Sneaky Window", &show_another_window);
		ImGui::Text("Hello from sneaky window!");

		if (ImGui::Button("Close me"))
		{
			show_another_window = false;
		}

		ImGui::End();
	}


	// Rendering all ImGui elements
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

	// -----------------------------------------------------
	
	return ret;
}

bool M_Editor::CleanUp()
{
	// ImGui CleanUp()
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
	return true;
}
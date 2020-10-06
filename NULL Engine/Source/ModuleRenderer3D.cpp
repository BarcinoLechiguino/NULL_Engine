#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "OpenGL.h"

#include "ImGui.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Source/Dependencies/glew/libx86/glew32.lib")

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module("Renderer3D", start_enabled), context()
{
	
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// Initializing glew.
	GLenum glew_init_return = glewInit();									// glew must be initialized after an OpenGL rendering context has been created.

	if (glew_init_return != GLEW_NO_ERROR)
	{
		LOG("GLEW could not initialize! SDL_Error: %s\n", SDL_GetError());
	}

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

	// Tweaking the platfor windows to look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setting up Platfor/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, context);
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

// PreUpdate: clear buffer
UPDATE_STATUS ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);

	glLoadMatrixf(App->camera->GetRawViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_STATUS::CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS ModuleRenderer3D::PostUpdate(float dt)
{
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

		if (ImGui::Button("STOP IT, GET SOME HELP"))
		{
			return UPDATE_STATUS::STOP;
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

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_STATUS::CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	// ImGui CleanUp()
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

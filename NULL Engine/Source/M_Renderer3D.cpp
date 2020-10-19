#include "Globals.h"
#include "Application.h"
#include "M_Window.h"
#include "M_Camera3D.h"
#include "Primitive.h"
#include "OpenGL.h"

#include "M_Input.h"

#include "M_Renderer3D.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

M_Renderer3D::M_Renderer3D(bool is_active) : Module("Renderer3D", is_active), context()
{

}

// Destructor
M_Renderer3D::~M_Renderer3D()
{}

// Called before render is available
bool M_Renderer3D::Init(Configuration& config)
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

	return ret;
}

// PreUpdate: clear buffer
UPDATE_STATUS M_Renderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);

	glLoadMatrixf(App->camera->GetRawViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
	{
		lights[i].Render();
	}

	return UPDATE_STATUS::CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS M_Renderer3D::PostUpdate(float dt)
{	
	PrimitiveExamples();

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_STATUS::CONTINUE;
}

// Called before quitting
bool M_Renderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void M_Renderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void M_Renderer3D::PrimitiveExamples()
{
	//OGL_draw_examples.DrawAllExamples();

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	//cube_direct.DirectRender();

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	//cube_array.ArrayRender();

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

	/*vec3 position = cube_indices.GetPos();

	if (App->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN)
	{
		position += vec3(0.0f, 1.0f, 0.0f);

		cube_indices.SetPos(position.x, position.y, position.z);
	}
	else if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		cube_indices.SetPos(position.x - 1.0f, position.y, position.z);
	}
	else if(App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
	{
		cube_indices.SetPos(position.x, position.y - 1.0f, position.z);
	}
	else if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		cube_indices.SetPos(position.x + 1.0f, position.y, position.z);
	}

	cube_indices.IndicesRender();*/

	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
	//sphere.IndiceRender(1, 12, 24);
	
	/*if (App->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN)
	{
		uint rings = sphere.GetRings();

		sphere.SetRings(rings + 1);
	}
	else if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
	{
		uint rings = sphere.GetRings();

		if (rings > 0)
		{
			sphere.SetRings(rings - 1);
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		uint sectors = sphere.GetSectors();

		sphere.SetSectors(sectors + 1);
	}
	else if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		uint sectors = sphere.GetSectors();

		if (sectors > 0)
		{
			sphere.SetSectors(sectors - 1);
		}
	}*/
	
	//sphere.IndiceRender();

	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	//pyramid.IndiceRender();

	glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
	//cylinder.InnerRender();

	cylinder.IndicesRender();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
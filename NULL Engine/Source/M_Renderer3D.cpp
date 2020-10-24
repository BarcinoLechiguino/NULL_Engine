
#include "Assimp.h"
#include "OpenGL.h"

#include "Globals.h"
#include "Application.h"
#include "M_Window.h"
#include "M_Camera3D.h"
#include "M_FileSystem.h"
#include "M_Editor.h"
#include "Primitive.h"

#include "R_Model.h"
#include "R_Mesh.h"
#include "I_Meshes.h"
#include "M_Input.h"																	//TMP

#include "M_Renderer3D.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

#pragma comment (lib, "Source/Dependencies/Assimp/libx86/assimp.lib")


M_Renderer3D::M_Renderer3D(bool is_active) : Module("Renderer3D", is_active), context()
{
	vsync = VSYNC;

	gl_depth_test		= true;
	gl_cull_face		= true;
	gl_lighting			= true;
	gl_color_material	= true;
	gl_texture_2D		= true;
	gl_show_normals		= false;
	gl_show_colors		= false;
	gl_show_tex_coords	= false;

}

// Destructor
M_Renderer3D::~M_Renderer3D()
{}

// Called before render is available
bool M_Renderer3D::Init(Configuration& config)
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	ret = InitOpenGL();															// Initializing OpenGL. (Context and initial configuration)

	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);										// Projection matrix recalculation to keep up with the resizing of the window.

	InitGlew();																	// Initializing Glew.

	LoadModel("Assets/Models/warrior/warrior.FBX");
	//LoadModel("Assets/Models/baker_house/BakerHouse.FBX");

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

	/*for (int i = 0; i < meshes.size(); ++i)
	{
		meshes[i]->Draw();
	}*/

	for (int i = 0; i < models.size(); ++i)
	{
		models[i]->Draw();
	}

	App->editor->RenderGuiPanels();

	SDL_GL_SwapWindow(App->window->GetWindow());

	return UPDATE_STATUS::CONTINUE;
}

// Called before quitting
bool M_Renderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	models.clear();

	SDL_GL_DeleteContext(context);

	return true;
}

bool M_Renderer3D::LoadConfiguration(Configuration& root)
{
	bool ret = true;

	return ret;
}

bool M_Renderer3D::SaveConfiguration(Configuration& root) const
{
	bool ret = true;

	return ret;
}

// ----------- RENDERER METHODS -----------
bool M_Renderer3D::InitOpenGL()
{
	bool ret = true;
	
	//Create OpenGL Context
	context = SDL_GL_CreateContext(App->window->GetWindow());
	if (context == NULL)
	{
		LOG("[ERROR] OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (ret == true)
	{
		//Use Vsync
		if (vsync && SDL_GL_SetSwapInterval(1) < 0)
		{
			LOG("[WARNING] Unable to set Vsync! SDL Error: %s\n", SDL_GetError());
		}

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("[ERROR] Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("[ERROR] Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("[ERROR] Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		lights[0].Active(true);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
	}

	return ret;
}

bool M_Renderer3D::InitGlew()
{
	bool ret = true;

	// Initializing glew.
	GLenum glew_init_return = glewInit();												// glew must be initialized after an OpenGL rendering context has been created.

	if (glew_init_return != GLEW_NO_ERROR)
	{
		LOG("[ERROR] GLEW could not initialize! SDL_Error: %s\n", SDL_GetError());

		ret = false;
	}

	return ret;
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

void M_Renderer3D::LoadModel(const char* file_path)
{
	R_Model* model = new R_Model();

	//const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

	char* buffer = nullptr;
	
	uint file_size = App->file_system->Load(file_path, &buffer);
	
	const aiScene* scene = aiImportFileFromMemory(buffer, file_size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);

	//aiImportFileFromMemory()

	if (scene != nullptr)
	{
		model->ProcessScene(scene);

		aiReleaseImport(scene);
	}

	models.push_back(model);
}

const char* M_Renderer3D::GetDrivers() const
{
	return SDL_GetCurrentVideoDriver();													// SDL_GetCurrentVideoDriver() returns NULL if no driver has been initialized.
}

bool M_Renderer3D::GetVsync() const
{
	return vsync;
}

void M_Renderer3D::SetVsync(bool set_to)
{
	if (set_to != vsync)
	{
		vsync = set_to;
		
		int result = SDL_GL_SetSwapInterval(vsync ? 1 : 0);								// 0 for immediate updates, 1 for updates synchronized with vertical retrace.

		if (result < 0)																	// SDL_GL_SetSwapInterval() returns 0 on success and -1 on failure.
		{
			LOG("[WARNING] Unable to set Vsync! SDL Error: %s\n", SDL_GetError()); 
		}
	}
}

bool M_Renderer3D::GetGLFlag(GLenum cap) const
{
	return glIsEnabled(cap);
}

void M_Renderer3D::SetGLFlag(GLenum cap, bool set_to)
{
	if (set_to != glIsEnabled(cap))
	{
		if (set_to)
		{
			glEnable(cap);
		}
		else
		{
			glDisable(cap);
		}
	}
}

bool M_Renderer3D::GetGLDepthTest() const
{
	return gl_depth_test;
}

bool M_Renderer3D::GetGLCullFace() const
{
	return gl_cull_face;
}

bool M_Renderer3D::GetGLLighting() const
{
	return gl_lighting;
}

bool M_Renderer3D::GetGLColorMaterial() const
{
	return gl_color_material;
}

bool M_Renderer3D::GetGLTexture2D() const
{
	return gl_texture_2D;
}

bool M_Renderer3D::GetGLShowNormals() const
{
	return gl_show_normals;
}

bool M_Renderer3D::GetGLShowColors() const
{
	return gl_show_colors;
}

bool M_Renderer3D::GetGLShowTexCoords() const
{
	return gl_show_tex_coords;
}

void M_Renderer3D::SetGLDepthTest(bool set_to)
{
	if (set_to != gl_depth_test)
	{
		gl_depth_test = set_to;

		if (gl_depth_test)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}
}

void M_Renderer3D::SetGLCullFace(bool set_to)
{
	if (set_to != gl_cull_face)
	{
		gl_cull_face = set_to;

		if (gl_cull_face)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
	}
}

void M_Renderer3D::SetGLLighting(bool set_to)
{
	if (set_to != gl_lighting)
	{
		gl_lighting = set_to;

		if (gl_lighting)
		{
			glEnable(GL_LIGHTING);
		}
		else
		{
			glDisable(GL_LIGHTING);
		}
	}
}

void M_Renderer3D::SetGLColorMaterial(bool set_to)
{
	if (set_to != gl_color_material)
	{
		gl_color_material = set_to;

		if (gl_lighting)
		{
			glEnable(GL_LIGHTING);
		}
		else
		{
			glDisable(GL_LIGHTING);
		}
	}
}

void M_Renderer3D::SetGLTexture2D(bool set_to)
{
	if (set_to != gl_texture_2D)
	{
		gl_texture_2D = set_to;

		if (gl_lighting)
		{
			glEnable(GL_TEXTURE_2D);
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
		}
	}
}

void M_Renderer3D::SetGLShowNormals(bool set_to)
{
	gl_show_normals = set_to;
}

void M_Renderer3D::SetGLShowColors(bool set_to)
{
	gl_show_colors = set_to;
}

void M_Renderer3D::SetGLShowTexCoords(bool set_to)
{
	gl_show_tex_coords = set_to;
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
	//pyramid.SetPos(5.0f, 0.0f, 0.0f);
	//pyramid.IndicesRender();

	glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
	//cylinder.InnerRender();
	//cylinder.SetPos(2.5f, 0.0f, 0.0f);
	//cylinder.IndicesRender();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
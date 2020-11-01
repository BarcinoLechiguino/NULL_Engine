#include "Assimp.h"
#include "OpenGL.h"

#include "Globals.h"
#include "Application.h"
#include "M_Window.h"
#include "M_Camera3D.h"
#include "M_Input.h"
#include "M_FileSystem.h"
#include "M_Editor.h"
#include "Primitive.h"

#include "R_Model.h"
#include "R_Mesh.h"
#include "R_Material.h"
#include "I_Materials.h"

#include "M_Renderer3D.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

#pragma comment (lib, "Source/Dependencies/Assimp/libx86/assimp.lib")


M_Renderer3D::M_Renderer3D(bool is_active) : Module("Renderer3D", is_active), context()
{
	vsync = VSYNC;

	draw_world_grid		= true;
	draw_world_axis		= true;
	show_wireframe		= false;
	show_normals		= false;
	show_tex_coords		= false;

	draw_primitive_examples = false;
	draw_primitives_by_indices = true;

	debug_texture_id	= 0;
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

	Importer::Materials::DevIL::Init();											// Initializing DevIL.
	
	CreatePrimitiveExamples();													// Adding one of each available primitice to the primitives vector for later display.

	//LoadModel("Assets/Models/teapot/teapot.FBX", vec4(0.0f, 1.0f, 1.0f, 1.0f));
	//LoadModel("Assets/Models/cube/small_cube.FBX", vec4(0.0f, 1.0f, 1.0f, 1.0f));
	//LoadModel("Assets/Models/baker_house/BakerHouse.FBX");

	LoadDebugTexture();

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
	lights[0].SetPos(App->camera->position.x, App->camera->position.y, App->camera->position.z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
	{
		lights[i].Render();
	}

	// --- RENDERER SHORTCUTS
	RendererShortcuts();

	return UPDATE_STATUS::CONTINUE;
}

// PostUpdate present buffer to screen
UPDATE_STATUS M_Renderer3D::PostUpdate(float dt)
{	
	if (draw_world_grid)
	{
		DrawWorldGrid(WORLD_GRID_SIZE);
	}

	if (draw_world_axis)
	{
		DrawWorldAxis();
	}
	
	for (uint i = 0; i < models.size(); ++i)
	{
		models[i]->Draw();

		if (show_normals)
		{
			models[i]->DrawNormals();
		}
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	if (draw_primitive_examples)
	{
		for (uint i = 0; i < primitives.size(); ++i)
		{
			if (!draw_primitives_by_indices)
			{
				primitives[i]->Render();
			}
			else
			{
				primitives[i]->RenderByIndices();
			}
		}
	}

	PrimitiveExamples();

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
		if (vsync)
		{
			if (SDL_GL_SetSwapInterval(1) < 0)
			{
				LOG("[ERROR] Unable to set Vsync! SDL Error: %s\n", SDL_GetError());
			}
			else
			{
				LOG("[STATUS] Vsync is activated!");
			}
		}
		else
		{
			if (SDL_GL_SetSwapInterval(0) < 0)
			{
				LOG("[ERROR] Unable to set frame update interval to immediate! SDL Error: %s\n", SDL_GetError());
			}
			else
			{
				LOG("[STATUS] Vsync is deactivated!")
			}
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

		SetGLFlag(GL_DEPTH_TEST, true);
		SetGLFlag(GL_CULL_FACE, true);
		SetGLFlag(GL_LIGHTING, true);
		SetGLFlag(GL_COLOR_MATERIAL, true);
		SetGLFlag(GL_TEXTURE_2D, true);
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

void M_Renderer3D::RendererShortcuts()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_STATE::KEY_DOWN)
	{
		draw_world_grid = !draw_world_grid;
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_STATE::KEY_DOWN)
	{
		draw_world_axis = !draw_world_axis;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_STATE::KEY_DOWN)
	{
		SetShowWireframe(!show_wireframe);
	}

	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_STATE::KEY_DOWN)
	{
		show_normals = !show_normals;
	}

	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_STATE::KEY_DOWN)
	{
		SetGLFlag(GL_TEXTURE_2D, !GetGLFlag(GL_TEXTURE_2D));
	}

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_STATE::KEY_DOWN)
	{
		SetGLFlag(GL_COLOR_MATERIAL, !GetGLFlag(GL_COLOR_MATERIAL));
	}
}

void M_Renderer3D::DrawWorldGrid(int size)
{
	glBegin(GL_LINES);

	float destination = (float)size;
	
	for (float origin = -destination; origin <= destination; origin += 1.0f)
	{
		glVertex3f( origin,		 0.0f, -destination);
		glVertex3f( origin,		 0.0f,  destination);
		glVertex3f(-destination, 0.0f,  origin);
		glVertex3f( destination, 0.0f,  origin);
	}

	glEnd();
}

void M_Renderer3D::DrawWorldAxis()
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);											// X Axis.
	glVertex3f(0.0f, 0.0f, 0.0f);		glVertex3f(1.0f,  0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f);		glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f);		glVertex3f(1.0f, -0.1f, 0.0f);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);											// Y Axis.
	glVertex3f( 0.0f,  0.0f,  0.0f);	glVertex3f(0.0f, 1.0f,  0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f);	glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f( 0.05f, 1.25f, 0.0f);	glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f( 0.0f,  1.15f, 0.0f);	glVertex3f(0.0f, 1.05f, 0.0f);

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);											// Z Axis.
	glVertex3f( 0.0f,   0.0f, 0.0f);	glVertex3f( 0.0f,   0.0f, 1.0f);
	glVertex3f(-0.05f,  0.1f, 1.05f);	glVertex3f( 0.05f,  0.1f, 1.05f);
	glVertex3f( 0.05f,  0.1f, 1.05f);	glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f);	glVertex3f( 0.05f, -0.1f, 1.05f);

	glEnd();

	glLineWidth(1.0f);
}

void M_Renderer3D::AddPrimitive(Primitive* primitive)
{
	primitives.push_back(primitive);
}

void M_Renderer3D::CreatePrimitiveExamples()
{
	Cube* cube			= new Cube();
	Sphere* sphere		= new Sphere();
	Cylinder* cylinder	= new Cylinder();
	Pyramid* pyramid	= new Pyramid();

	sphere->SetPos(2.0f, 0.0f, 0.0f);
	cylinder->SetPos(4.0f, 0.0f, 0.0f);
	pyramid->SetPos(12.0f, 0.0f, 0.0f);

	primitives.push_back(cube);
	primitives.push_back(sphere);
	primitives.push_back(cylinder);
	primitives.push_back(pyramid);
}

void M_Renderer3D::LoadModel(const char* file_path, vec4 mat_colour)
{
	R_Model* model = new R_Model(file_path, mat_colour);

	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

	//char* buffer = nullptr;
	//uint file_size = App->file_system->Load(file_path, &buffer);
	//const aiScene* scene = aiImportFileFromMemory(buffer, file_size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);

	if (scene != nullptr)
	{
		model->ProcessScene(scene);

		aiReleaseImport(scene);
	}

	models.push_back(model);
}

void M_Renderer3D::GenerateBuffers(R_Mesh* mesh)
{
	if (!mesh->vertices.empty())
	{
		glGenBuffers(1, (GLuint*)&mesh->VBO);																			// Generates the Vertex Buffer Object
		glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);																		// Binds VBO with the GL_ARRAY_BUFFER biding point (target):
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->vertices.size(), &mesh->vertices[0], GL_STATIC_DRAW);		// Inits the data stored inside VBO and specifies how the data will be accessed.
	}

	if (!mesh->normals.empty())
	{
		glGenBuffers(1, (GLuint*)&mesh->NBO);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->NBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->normals.size(), &mesh->normals[0], GL_STATIC_DRAW);
	}

	if (!mesh->tex_coords.empty())
	{
		glGenBuffers(1, (GLuint*)&mesh->TBO);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->TBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->tex_coords.size(), &mesh->tex_coords[0], GL_STATIC_DRAW);
	}

	if (!mesh->indices.empty())
	{
		glGenBuffers(1, (GLuint*)&mesh->IBO);																			// Generates the Index Buffer Object
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IBO);																// Binds IBO with the GL_ARRAY_BUFFER biding point (target):
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->indices.size(), &mesh->indices[0], GL_STATIC_DRAW);	// Inits the data stored inside IBO and specifies how the data will be accessed.
	}
}

void M_Renderer3D::DrawMesh(R_Mesh* mesh)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, debug_texture_id);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->TBO);
	glTexCoordPointer(2, GL_FLOAT, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);																// The vertex buffer is bound so the vertex positions can be interpreted correctly.
	glVertexPointer(3, GL_FLOAT, 0, nullptr);														// Here the size and the type of the vertex is defined.

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IBO);
	//glColor4f(colour.r, colour.g, colour.b, colour.a);
	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	// --- DEBUG DRAW ---
	if (mesh->draw_normals)
	{
		mesh->DrawNormals();
	}
}

void M_Renderer3D::LoadDebugTexture()
{	
	GLubyte checker_image[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];					// HEIGHT columns, WIDTH rows and 4 variables per checker (for RGBA purposes).

	for (int i = 0; i < CHECKERS_HEIGHT; ++i)									// There will be CHECKERS_WIDTH rows per column.
	{
		for (int j = 0; j < CHECKERS_WIDTH; ++j)								// There will be an RGBA value per checker.
		{
			int color = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;			// Getting whether the checker will be white or black according to the iteration indexes and bitwise operations.

			checker_image[i][j][0] = (GLubyte)color;							// R
			checker_image[i][j][1] = (GLubyte)color;							// G
			checker_image[i][j][2] = (GLubyte)color;							// B
			checker_image[i][j][3] = (GLubyte)255;								// A

			//LOG("CHECKER COLOR: %s", color == 255 ? "White" : "Black");
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);										// Sets the pixel storage modes. GL_UNPACK_ALIGNMENT specifies the alignment requirements for the
	// --->																		// start of each pixel row in memory. 1 means that the alignment requirements will be byte-alignment.
	glGenTextures(1, &debug_texture_id);										// Generate texture names. Returns n names in the given buffer. GL_INVALID_VALUE if n is negative.
	glBindTexture(GL_TEXTURE_2D, debug_texture_id);								// Bind a named texture to a texturing target. Binds the buffer with the given target (GL_TEXTURE_2D).
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);				// Set texture parameters. WRAP_S: Set the wrap parameters for texture coordinate s. GL_REPEAT is the default.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);				// WRAP_T: Set the wrap parameters for the texture coordinate r
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);			// MAG_FILTER: Sets the texture magnification process parameters. GL_NEAREST rets the val. of nearest tex elem.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);			// MIN_FILTER: Sets the texture minimization process parameters. " ".
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	// MIPMAP_NEAREST: Same as GL_NEAREST but works with the mipmaps generated by glGenerateMipmap() to
	// --->																				// handle the process of resizing a tex. Takes the mipmap that most closely matches the size of the px.

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checker_image);

	//glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
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
		else
		{
			LOG("[STATUS] Vsync has been %s", vsync ? "activated" : "deactivated");
		}
	}
}

bool M_Renderer3D::GetGLFlag(GLenum cap) const
{
	return glIsEnabled(cap);
}

bool M_Renderer3D::GetGLFlag(RENDERER_FLAGS cap) const
{
	return glIsEnabled((GLenum)cap);
}

void M_Renderer3D::SetGLFlag(GLenum cap, bool set_to)
{
	if (set_to != (bool)glIsEnabled(cap))
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

void M_Renderer3D::SetGLFlag(RENDERER_FLAGS cap, bool set_to)
{
	GLenum flag = (GLenum)cap;
	
	if (set_to != (bool)glIsEnabled(flag))
	{
		if (set_to)
		{
			glEnable(flag);
		}
		else
		{
			glDisable(flag);
		}
	}
}

bool M_Renderer3D::GetDrawWorldGrid() const
{
	return draw_world_grid;
}

bool M_Renderer3D::GetDrawWorldAxis() const
{
	return draw_world_axis;
}

bool M_Renderer3D::GetShowWireframe() const
{
	return show_wireframe;
}

bool M_Renderer3D::GetShowNormals() const
{
	return show_normals;
}

bool M_Renderer3D::GetShowTexCoords() const
{
	return show_tex_coords;
}

bool M_Renderer3D::GetDrawPrimitiveExamples() const
{
	return draw_primitive_examples;
}

void M_Renderer3D::SetDrawWorldGrid(bool set_to)
{
	if (set_to != draw_world_grid)
	{
		draw_world_grid = set_to;
	}
}

void M_Renderer3D::SetDrawWorldAxis(bool set_to)
{
	if (set_to != draw_world_axis)
	{
		draw_world_axis = set_to;
	}
}

void M_Renderer3D::SetShowWireframe(bool set_to)
{
	if (set_to != show_wireframe)
	{
		show_wireframe = set_to;

		if (show_wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}

void M_Renderer3D::SetShowNormals(bool set_to)
{
	if (set_to != show_normals)
	{
		show_normals = set_to;
	}
}

void M_Renderer3D::SetShowTexCoords(bool set_to)
{
	if (set_to != show_tex_coords)
	{
		show_tex_coords = set_to;
	}
}

void M_Renderer3D::SetDrawPrimtiveExamples(bool set_to)
{
	if (set_to != draw_primitive_examples)
	{
		draw_primitive_examples = set_to;
	}
}

void M_Renderer3D::PrimitiveExamples()
{	
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	//glBindTexture(GL_TEXTURE_2D, debug_texture_id);
	//cube_direct.DirectRender();
	//glBindTexture(GL_TEXTURE_2D, 0);

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	//glBindTexture(GL_TEXTURE_2D, debug_texture_id);
	//cube_array.ArrayRender();
	//glBindTexture(GL_TEXTURE_2D, 0);

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	/*cube_indices.IndicesRender();*/

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
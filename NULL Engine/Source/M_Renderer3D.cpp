#include "Profiler.h"																			// Libraries & Other 3rd Party Softwares
#include "OpenGL.h"																				// -------------------------------------

#include "Color.h"																				// Containers
#include "Primitive.h"																			// ----------

#include "Application.h"																		// Application and Modules
#include "M_Window.h"																			// 
#include "M_Camera3D.h"																			// 
#include "M_Input.h"																			// 
#include "M_FileSystem.h"																		// 
#include "M_Editor.h"																			// -----------------------

#include "R_Mesh.h"																				// Resources
#include "R_Material.h"																			//
#include "R_Texture.h"																			// ---------

#include "I_Textures.h"																			// Importers

#include "C_Mesh.h"																				// GameObject and Components Trading these two deps. for a much cleaner RenderMesh() method.
#include "C_Material.h"																			// -------------------------

#include "M_Renderer3D.h"																		// Header of this .cpp file.

#include "MemoryManager.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */							// Libraries Pragma Comments
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */							// 
#pragma comment (lib, "Source/Dependencies/Assimp/libx86/assimp.lib")							// -------------------------

#define WORLD_GRID_SIZE 100
#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64

M_Renderer3D::M_Renderer3D(bool is_active) : Module("Renderer3D", is_active), 
context					(),
vsync					(VSYNC),
draw_world_grid			(true),
draw_world_axis			(true),
in_wireframe_mode		(false),
draw_primitive_examples	(false),
scene_framebuffer		(0),
depth_buffer			(0),
scene_render_texture	(0),
depth_buffer_texture	(0),
game_framebuffer		(0),
debug_texture_id		(0)
{

}

// Destructor
M_Renderer3D::~M_Renderer3D()
{

}

// Called before render is available
bool M_Renderer3D::Init(ParsonNode& configuration)
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	ret = InitOpenGL();																			// Initializing OpenGL. (Context and initial configuration)

	OnResize();																					// Projection matrix recalculation to keep up with the resizing of the window.
	
	Importer::Textures::Init();																	// Initializing DevIL.

	CreatePrimitiveExamples();																	// Adding one of each available primitice to the primitives vector for later display.

	//InitFramebuffers();
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
	BROFILER_CATEGORY("M_Renderer3D PostUpdate", Profiler::Color::Chartreuse);
	
	RenderScene();

	App->editor->RenderEditorPanels();

	SDL_GL_SwapWindow(App->window->GetWindow());

	return UPDATE_STATUS::CONTINUE;
}

// Called before quitting
bool M_Renderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	for (uint i = 0; i < primitives.size(); ++i)
	{
		RELEASE(primitives[i]);
	}
	
	primitives.clear();

	Importer::Textures::CleanUp();																// Shutting down DevIL. 

	FreeBuffers();

	SDL_GL_DeleteContext(context);

	return true;
}

bool M_Renderer3D::LoadConfiguration(ParsonNode& root)
{
	bool ret = true;

	return ret;
}

bool M_Renderer3D::SaveConfiguration(ParsonNode& root) const
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

	ret = InitGlew();																					// Initializing Glew.

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
				LOG("[STATUS] Vsync is deactivated!");
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
		glClear(GL_COLOR_BUFFER_BIT);

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

		SetGLFlag(GL_ALPHA_TEST, true);
		glAlphaFunc(GL_GREATER, 0.20f);													// Have alpha test in c_material (color alpha)?

		SetGLFlag(GL_BLEND, true);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

void M_Renderer3D::OnResize()
{	
	uint win_width	= App->window->GetWidth();
	uint win_height = App->window->GetHeight();
	
	glViewport(0, 0, win_width, win_width);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	projection_matrix = perspective(60.0f, (float)win_width / (float)win_height, 0.125f, 512.0f);
	//projection_matrix = float4x4::OpenGLPerspProjRH(60.0f, (float)width / (float)height, 0.125f, 512.0f); /*perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);*/
	glLoadMatrixf((GLfloat*)&projection_matrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	InitFramebuffers();
}

void M_Renderer3D::InitFramebuffers()
{
	glGenFramebuffers(1, (GLuint*)&scene_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, scene_framebuffer);
	
	// --- SCENE RENDER TEXTURE ---
	glGenTextures(1, (GLuint*)&scene_render_texture);
	glBindTexture(GL_TEXTURE_2D, scene_render_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, App->window->GetWidth(), App->window->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// --- DEPTH BUFFER TEXTURE ---
	glGenTextures(1, (GLuint*)&depth_buffer_texture);
	glBindTexture(GL_TEXTURE_2D, depth_buffer_texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	//glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT, App->window->GetWidth(), App->window->GetHeight());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, App->window->GetWidth(), App->window->GetHeight(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, scene_render_texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_buffer_texture, 0);

	// --- DEPTH & STENCIL BUFFERS ---
	glGenRenderbuffers(1, (GLuint*)&depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
	
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, App->window->GetWidth(), App->window->GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG("[ERROR] Renderer 3D: Could not generate the scene's frame buffer! Error: %s", gluErrorString(glGetError()));
	}

	// --- UNBINDING THE FRAMEBUFFER ---
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void M_Renderer3D::LoadDebugTexture()
{
	GLubyte checker_image[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];							// HEIGHT columns, WIDTH rows and 4 variables per checker (for RGBA purposes).

	for (int i = 0; i < CHECKERS_HEIGHT; ++i)											// There will be CHECKERS_WIDTH rows per column.
	{
		for (int j = 0; j < CHECKERS_WIDTH; ++j)										// There will be an RGBA value per checker.
		{
			int color = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;					// Getting the checker's color (white or black) according to the iteration indices and bitwise ops.

			checker_image[i][j][0] = (GLubyte)color;									// R
			checker_image[i][j][1] = (GLubyte)color;									// G
			checker_image[i][j][2] = (GLubyte)color;									// B
			checker_image[i][j][3] = (GLubyte)255;										// A
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);												// Sets the pixel storage modes. GL_UNPACK_ALIGNMENT specifies the alignment requirements for the
	// --->																				// start of each pixel row in memory. 1 means that the alignment requirements will be byte-alignment.
	glGenTextures(1, &debug_texture_id);												// Generate texture names. Returns n names in the given buffer. GL_INVALID_VALUE if n is negative.
	glBindTexture(GL_TEXTURE_2D, debug_texture_id);										// Bind a named texture to a texturing target. Binds the buffer with the given target (GL_TEXTURE_2D).

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);						// Set texture parameters. WRAP_S: Set the wrap parameters for tex. coord. S.. GL_REPEAT is the default.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);						// WRAP_T: Set the wrap parameters for the texture coordinate r
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);					// MAG_FILTER: Sets the tex. magnification process param.. GL_NEAREST rets the val. of nearest tex elem.
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);					// MIN_FILTER: Sets the texture minimization process parameters. " ".
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	// MIPMAP_NEAREST: Same as GL_NEAREST but works with the mipmaps generated by glGenerateMipmap() to
	// --->																				// handle the process of resizing a tex. Takes the mipmap that most closely matches the size of the px.

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checker_image);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void M_Renderer3D::FreeBuffers()
{
	glDeleteRenderbuffers(1, (GLuint*)&depth_buffer);
	glDeleteTextures(1, (GLuint*)&scene_render_texture);
	glDeleteFramebuffers(1, (GLuint*)&scene_framebuffer);

	glDeleteTextures(1, (GLuint*)&debug_texture_id);
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
		SetInWireframeMode(!in_wireframe_mode);
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

void M_Renderer3D::RenderScene()
{	
	glBindFramebuffer(GL_FRAMEBUFFER, scene_framebuffer);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	if (draw_world_grid)
	{
		DrawWorldGrid(WORLD_GRID_SIZE);
	}

	if (draw_world_axis)
	{
		DrawWorldAxis();
	}

	RenderMeshes();
	RenderCuboids();

	//PrimitiveDrawExamples p_ex = PrimitiveDrawExamples();
	//p_ex.DrawAllExamples();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	if (draw_primitive_examples)
	{
		for (uint i = 0; i < primitives.size(); ++i)
		{
			primitives[i]->RenderByIndices();
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glLineWidth(1.0f);
}

void M_Renderer3D::AddRenderersBatch(const std::vector<MeshRenderer>& mesh_renderers, const std::vector<CuboidRenderer>& cuboid_renderers)
{	
	for (uint i = 0; i < mesh_renderers.size(); ++i)
	{
		this->mesh_renderers.push_back(mesh_renderers[i]);
	}

	for (uint i = 0; i < cuboid_renderers.size(); ++i)
	{
		this->cuboid_renderers.push_back(cuboid_renderers[i]);
	}

	//this->mesh_renderers.resize(mesh_renderers.size());
	//this->cuboid_renderers.resize(cuboid_renderers.size());
	//memcpy(&this->mesh_renderers[0], &mesh_renderers[0], mesh_renderers.size());
	//memcpy(&this->cuboid_renderers[0], &cuboid_renderers[0], cuboid_renderers.size());
}

void M_Renderer3D::RenderMeshes()
{
	for (uint i = 0; i < mesh_renderers.size(); ++i)
	{
		mesh_renderers[i].Render();
	}

	mesh_renderers.clear();
}

void M_Renderer3D::RenderCuboids()
{
	glLineWidth(3.0f);
	
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	//glBegin(GL_LINE_STRIP);

	for (uint i = 0; i < cuboid_renderers.size(); ++i)
	{	
		cuboid_renderers[i].Render();
	}

	cuboid_renderers.clear();

	glEnd();
	glEnable(GL_LIGHTING);

	glLineWidth(1.0f);
}

void M_Renderer3D::ClearRenderers()
{
	mesh_renderers.clear();
	cuboid_renderers.clear();
}

void M_Renderer3D::AddPrimitive(Primitive* primitive)
{
	primitives.push_back(primitive);
}

void M_Renderer3D::CreatePrimitiveExamples()
{
	P_Cube* cube			= new P_Cube();
	P_Sphere* sphere		= new P_Sphere();
	P_Cylinder* cylinder	= new P_Cylinder();
	P_Pyramid* pyramid		= new P_Pyramid();

	sphere->SetPos(2.0f, 0.0f, 0.0f);
	cylinder->SetPos(4.0f, 0.0f, 0.0f);
	pyramid->SetPos(12.0f, 0.0f, 0.0f);

	primitives.push_back(cube);
	primitives.push_back(sphere);
	primitives.push_back(cylinder);
	primitives.push_back(pyramid);
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

void M_Renderer3D::RenderMesh(float4x4 transform, C_Mesh* c_mesh, C_Material* c_material)
{	
	//glBindFramebuffer(GL_FRAMEBUFFER, scene_framebuffer);
	//glClearColor(0.278f, 0.278f, 0.278f, 0.278f);
	//glClear(GL_COLOR_BUFFER_BIT);
	//glClear(GL_DEPTH_BUFFER_BIT);


	R_Mesh* r_mesh = c_mesh->GetMesh();
	
	if (in_wireframe_mode)
	{
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	}

	//glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf((GLfloat*)&transform.Transposed());												// OpenGL requires that the 4x4 matrices are column-major instead of row-major.

	if (c_material != nullptr)
	{
		if (!c_material->IsActive())
		{
			SetGLFlag(GL_TEXTURE_2D, false);
		}

		if (c_material->GetTexture() == nullptr)														// If the Material Component does not have a Texture Resource.
		{
			Color color = c_material->GetMaterialColour();
			glColor4f(color.r, color.g, color.b, color.a);												// Apply the diffuse color to the mesh.
		}
		else if (c_material->UseDefaultTexture())														// If the default texture is set to be used (bool use_default_texture)
		{
			glBindTexture(GL_TEXTURE_2D, debug_texture_id);												// Binding the texture that will be rendered. Index = 0 means we are clearing the binding.
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, c_material->GetTextureID());									// Binding the texture_id in the Texture Resource of the Material Component.
		}
	}

	glEnableClientState(GL_VERTEX_ARRAY);															// Enables the vertex array for writing and to be used during rendering.
	glEnableClientState(GL_NORMAL_ARRAY);															// Enables the normal array for writing and to be used during rendering.
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);													// Enables the texture coordinate array for writing and to be used during rendering.

	glBindBuffer(GL_ARRAY_BUFFER, r_mesh->TBO);														// Will bind the buffer object with the mesh->TBO identifyer for rendering.
	glTexCoordPointer(2, GL_FLOAT, 0, nullptr);														// Specifies the location and data format of an array of tex coords to use when rendering.

	glBindBuffer(GL_ARRAY_BUFFER, r_mesh->NBO);														// The normal buffer is bound so the normal positions can be interpreted correctly.
	glNormalPointer(GL_FLOAT, 0, nullptr);															// 

	glBindBuffer(GL_ARRAY_BUFFER, r_mesh->VBO);														// The vertex buffer is bound so the vertex positions can be interpreted correctly.
	glVertexPointer(3, GL_FLOAT, 0, nullptr);														// Specifies the location and data format of an array of vert coords to use when rendering.

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r_mesh->IBO);												// Will bind the buffer object with the mesh->IBO identifyer for rendering.
	glDrawElements(GL_TRIANGLES, r_mesh->indices.size(), GL_UNSIGNED_INT, nullptr);					// 

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);														// Clearing the buffers.
	glBindBuffer(GL_ARRAY_BUFFER, 0);																// 												
	glBindTexture(GL_TEXTURE_2D, 0);																// ---------------------

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);													// Disabling the client-side capabilities enabled at the beginning.
	glDisableClientState(GL_NORMAL_ARRAY);															// 
	glDisableClientState(GL_VERTEX_ARRAY);															// Disabling GL_TEXTURE_COORD_ARRAY, GL_NORMAL_ARRAY and GL_VERTEX_ARRAY.

	if (c_material != nullptr)
	{
		if (!c_material->IsActive())
		{
			SetGLFlag(GL_TEXTURE_2D, true);
		} 
	}

	// --- DEBUG DRAW ---
	if (r_mesh->draw_vertex_normals)
	{
		r_mesh->DrawVertexNormals();
	}

	if (r_mesh->draw_face_normals)
	{
		r_mesh->DrawFaceNormals();
	}

	glPopMatrix();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClearColor(0.278f, 0.278f, 0.278f, 0.278f);
}

//float4x4 M_Renderer3D::GetProjectionMatrix() const
//{
//	return projection_matrix;
//}
//
//float3x3 M_Renderer3D::GetNormalMatrix() const
//{
//	return normal_matrix;
//}

mat4x4 M_Renderer3D::GetProjectionMatrix() const
{
	return projection_matrix;
}

mat3x3 M_Renderer3D::GetNormalMatrix() const
{
	return normal_matrix;
}

uint M_Renderer3D::GetDebugTextureID() const
{
	return debug_texture_id;
}

uint M_Renderer3D::GetSceneFramebuffer() const
{
	return scene_framebuffer;
}

uint M_Renderer3D::GetSceneRenderTexture() const
{
	return scene_render_texture;
}

uint M_Renderer3D::GetGameFramebuffer() const
{
	return game_framebuffer;
}

uint M_Renderer3D::GetDepthBuffer() const
{
	return depth_buffer;
}

uint M_Renderer3D::GetDepthBufferTexture() const
{
	return depth_buffer_texture;
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

bool M_Renderer3D::GetGLFlag(GLenum flag) const
{
	return glIsEnabled(flag);
}

bool M_Renderer3D::GetGLFlag(RENDERER_FLAGS flag) const
{
	return glIsEnabled((GLenum)flag);
}

void M_Renderer3D::SetGLFlag(GLenum flag, bool set_to)
{
	if (set_to != (bool)glIsEnabled(flag))
	{
		set_to ? glEnable(flag) : glDisable(flag);
	}
}

void M_Renderer3D::SetGLFlag(RENDERER_FLAGS flag, bool set_to)
{
	GLenum gl_flag = (GLenum)flag;
	
	if (set_to != (bool)glIsEnabled(gl_flag))
	{
		set_to ? glEnable(gl_flag) : glDisable(gl_flag);
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

bool M_Renderer3D::GetInWireframeMode() const
{
	return in_wireframe_mode;
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

void M_Renderer3D::SetInWireframeMode(bool set_to)
{
	if (set_to != in_wireframe_mode)
	{
		in_wireframe_mode = set_to;

		if (in_wireframe_mode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			SetGLFlag(GL_TEXTURE_2D, false);
			SetGLFlag(GL_LIGHTING, false);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			SetGLFlag(GL_TEXTURE_2D, true);
			SetGLFlag(GL_LIGHTING, true);
		}
	}
}

void M_Renderer3D::SetDrawPrimtiveExamples(bool set_to)
{
	if (set_to != draw_primitive_examples)
	{
		draw_primitive_examples = set_to;
	}
}

// --- RENDERER STRUCTURES METHODS ---
MeshRenderer::MeshRenderer(const float4x4& transform, C_Mesh* c_mesh, C_Material* c_material) :
transform	(transform),
c_mesh		(c_mesh),
c_material	(c_material)
{

}

void MeshRenderer::Render()
{
	R_Mesh* r_mesh = c_mesh->GetMesh();

	if (r_mesh == nullptr)
	{
		LOG("[ERROR] Renderer 3D: Could not render mesh! Error: R_Mesh* was nullptr.");
		return;
	}

	//glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf((GLfloat*)&transform.Transposed());												// OpenGL requires that the 4x4 matrices are column-major instead of row-major.

	ApplyDebugParameters();																			// Enable Wireframe Mode for this specific mesh, etc.
	ApplyTextureAndMaterial();																		// Apply resource texture or default texture, mesh color...

	glEnableClientState(GL_VERTEX_ARRAY);															// Enables the vertex array for writing and to be used during rendering.
	glEnableClientState(GL_NORMAL_ARRAY);															// Enables the normal array for writing and to be used during rendering.
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);													// Enables the texture coordinate array for writing and to be used during rendering.

	glBindBuffer(GL_ARRAY_BUFFER, r_mesh->TBO);														// Will bind the buffer object with the mesh->TBO identifyer for rendering.
	glTexCoordPointer(2, GL_FLOAT, 0, nullptr);														// Specifies the location and data format of an array of tex coords to use when rendering.

	glBindBuffer(GL_ARRAY_BUFFER, r_mesh->NBO);														// The normal buffer is bound so the normal positions can be interpreted correctly.
	glNormalPointer(GL_FLOAT, 0, nullptr);															// 

	glBindBuffer(GL_ARRAY_BUFFER, r_mesh->VBO);														// The vertex buffer is bound so the vertex positions can be interpreted correctly.
	glVertexPointer(3, GL_FLOAT, 0, nullptr);														// Specifies the location and data format of an array of vert coords to use when rendering.

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r_mesh->IBO);												// Will bind the buffer object with the mesh->IBO identifyer for rendering.
	glDrawElements(GL_TRIANGLES, r_mesh->indices.size(), GL_UNSIGNED_INT, nullptr);					// 

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);														// Clearing the buffers.
	glBindBuffer(GL_ARRAY_BUFFER, 0);																// 												
	glBindTexture(GL_TEXTURE_2D, 0);																// ---------------------

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);													// Disabling the client-side capabilities enabled at the beginning.
	glDisableClientState(GL_NORMAL_ARRAY);															// 
	glDisableClientState(GL_VERTEX_ARRAY);															// Disabling GL_TEXTURE_COORD_ARRAY, GL_NORMAL_ARRAY and GL_VERTEX_ARRAY.

	ClearTextureAndMaterial();																		// Clear the specifications applied in ApplyTextureAndMaterial().
	ClearDebugParameters();																			// Clear the specifications applied in ApplyDebugParameters().

	// --- DEBUG DRAW ---
	if (r_mesh->draw_vertex_normals)
	{
		r_mesh->DrawVertexNormals();
	}

	if (r_mesh->draw_face_normals)
	{
		r_mesh->DrawFaceNormals();
	}

	glPopMatrix();
}

void MeshRenderer::ApplyDebugParameters()
{
	if (App->renderer->GetInWireframeMode() || c_mesh->GetShowWireframe())
	{
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	}

	if (c_mesh->GetShowWireframe() && !App->renderer->GetInWireframeMode())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
	}
}

void MeshRenderer::ClearDebugParameters()
{
	if (c_mesh->GetShowWireframe() && !App->renderer->GetInWireframeMode())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void MeshRenderer::ApplyTextureAndMaterial()
{
	if (c_material != nullptr)
	{
		if (!c_material->IsActive())
		{
			glDisable(GL_TEXTURE_2D);
		}

		if (c_material->GetTexture() == nullptr)														// If the Material Component does not have a Texture Resource.
		{
			Color color = c_material->GetMaterialColour();
			glColor4f(color.r, color.g, color.b, color.a);												// Apply the diffuse color to the mesh.
		}
		else if (c_material->UseDefaultTexture())														// If the default texture is set to be used (bool use_default_texture)
		{
			glBindTexture(GL_TEXTURE_2D, App->renderer->GetDebugTextureID());							// Binding the texture that will be rendered. Index = 0 means we are clearing the binding.
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, c_material->GetTextureID());									// Binding the texture_id in the Texture Resource of the Material Component.
		}
	}
}

void MeshRenderer::ClearTextureAndMaterial()
{
	if (c_material != nullptr)
	{
		if (!c_material->IsActive())
		{
			glEnable(GL_TEXTURE_2D);
		}
	}
}


CuboidRenderer::CuboidRenderer(const float3* vertices, Color color) :
vertices(vertices),
color(color)
{

}

void CuboidRenderer::Render()
{
	glColor4f(color.r, color.g, color.b, color.a);

	// For a Cuboid with vertices ABCDEFGH
	GLfloat* A = (GLfloat*)&vertices[0];
	GLfloat* B = (GLfloat*)&vertices[1];
	GLfloat* C = (GLfloat*)&vertices[2];
	GLfloat* D = (GLfloat*)&vertices[3];
	GLfloat* E = (GLfloat*)&vertices[4];
	GLfloat* F = (GLfloat*)&vertices[5];
	GLfloat* G = (GLfloat*)&vertices[6];
	GLfloat* H = (GLfloat*)&vertices[7];

	//glBegin(GL_LINES);

	// --- FRONT
	glVertex3fv(A);											// BOTTOM HORIZONTAL										// Firstly the Near Plane is constructed.
	glVertex3fv(B);											// -----------------
	glVertex3fv(D);											// TOP HORIZONTAL
	glVertex3fv(C);											// -------------

	glVertex3fv(B);											// LEFT VERTICAL
	glVertex3fv(D);											// -------------
	glVertex3fv(C);											// RIGHT VERTICAL
	glVertex3fv(A);											// --------------

	// --- BACK
	glVertex3fv(F);											// BOTTOM HORIZONTAL										// Secondly the Far Plane is constructed.
	glVertex3fv(E);											// -----------------
	glVertex3fv(G);											// TOP HORIZONTAL
	glVertex3fv(H);											// -------------- 

	glVertex3fv(E);											// LEFT VERTICAL 
	glVertex3fv(G);											// ------------- 
	glVertex3fv(H);											// RIGHT VERTICAL 
	glVertex3fv(F);											// -------------- 

	// --- RIGHT
	glVertex3fv(F);											// BOTTOM HORIZONTAL										// Lastly, the Near and Far Planes' corners are connected.
	glVertex3fv(B); 										// -----------------
	glVertex3fv(H); 										// TOP HORIZONTAL 
	glVertex3fv(D); 										// -------------- 
	
	// --- LEFT
	glVertex3fv(E);											// BOTTOM HORIZONTAL										// ---
	glVertex3fv(A);											// -----------------
	glVertex3fv(C);											// TOP HORIZONTAL 
	glVertex3fv(G);											// -------------- 

	//glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

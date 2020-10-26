#ifndef __M_RENDERER_3D_H__
#define __M_RENDERER_3D_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#include "Primitive.h"															// REMOVE LATER

#include "ImGui/imgui.h"

#define MAX_LIGHTS 8
#define WORLD_GRID_SIZE 200
#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64

class Configuration;
class R_Mesh;
class R_Model;

class aiScene;
typedef unsigned int GLenum;

enum class RENDERER_FLAGS
{
	DEPTH_TEST		= 0x0B71,
	CULL_FACE		= 0x0B44,
	LIGHTING		= 0x0B50,
	COLOR_MATERIAL	= 0x0B57,
	TEXTURE_2D		= 0x0DE1,
};

class M_Renderer3D : public Module
{
public:
	M_Renderer3D(bool is_active = true);
	~M_Renderer3D();

	bool			Init(Configuration& config) override;
	UPDATE_STATUS	PreUpdate(float dt) override;
	UPDATE_STATUS	PostUpdate(float dt) override;
	bool			CleanUp() override;
	
	bool			LoadConfiguration(Configuration& root) override;
	bool			SaveConfiguration(Configuration& root) const override;

public:
	bool			InitOpenGL();
	bool			InitGlew();
	void			OnResize(int width, int height);
	
	void			RendererShortcuts();

	void			DrawWorldGrid(int size);
	void			DrawWorldAxis();

	void			AddPrimitive(Primitive* primitive);
	void			LoadModel(const char* file_path, vec4 mat_colour = vec4(1.0f, 1.0f, 1.0f, 1.0f));
	void			LoadDebugTexture();
	void			CorrectAxisAlignment(aiScene* scene);

	void			PrimitiveExamples();									// REMOVE LATER

public:																		// --- GET/SET METHODS
	const char*		GetDrivers() const;										// 
	bool			GetVsync() const;										// 
	void			SetVsync(bool set_to);									// 

	bool			GetGLFlag(GLenum cap) const;							// 
	bool			GetGLFlag(RENDERER_FLAGS cap) const;					// 
	void			SetGLFlag(GLenum cap, bool set_to);						// 
	void			SetGLFlag(RENDERER_FLAGS cap, bool set_to);				// 

	bool			GetDrawWorldGrid() const;								//
	bool			GetDrawWorldAxis() const;								//
	bool			GetShowWireframe() const;								//
	bool			GetShowNormals() const;									// 
	bool			GetShowTexCoords() const;								//
	void			SetDrawWorldGrid(bool set_to);
	void			SetDrawWorldAxis(bool set_to);
	void			SetShowWireframe(bool set_to);							//
	void			SetShowNormals(bool set_to);							// 
	void			SetShowTexCoords(bool set_to);							// 

public:
	Light					lights[MAX_LIGHTS];								// 
	SDL_GLContext			context;										// 
	mat3x3					NormalMatrix;									// 
	mat4x4					ProjectionMatrix;								// 

	uint					debug_texture_id;

	std::vector<R_Model*>	models;
	std::vector<Primitive*>	primitives;

private:
	bool					vsync;											// Will keep track of whether or not the vsync is currently active.

	bool					draw_world_grid;								//
	bool					draw_world_axis;								//
	bool					show_wireframe;									//
	bool					show_normals;									// 
	bool					show_tex_coords;								// 

	PrimitiveDrawExamples	OGL_draw_examples;								// REMOVE LATER
	Cube					cube_direct;									// 
	Cube					cube_array;										// 
	Cube					cube_indices;									// 
	//Sphere				sphere;											// 								
	Pyramid					pyramid;										// 								
	//Cylinder				cylinder;										// ------------
};

#endif // !__M_RENDERER_3D_H__
#ifndef __M_RENDERER_3D_H__
#define __M_RENDERER_3D_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#include "Primitive.h"															// REMOVE LATER

#include "ImGui/imgui.h"

#define MAX_LIGHTS 8

class Configuration;
class R_Mesh;
class R_Model;

typedef unsigned int GLenum;

enum class RENDERER_FLAGS
{
	DEPTH_TEST		= 0x0B71,
	CULL_FACE		= 0x0B44,
	LIGHTING		= 0x0B50,
	COLOR_MATERIAL	= 0x0B57,
	TEXTURE_2D		= 0x0DE1,
	SHOW_WIREFRAME	= -1,
	SHOW_NORMALS	= 0,
	SHOW_COLORS		= 1,
	SHOW_TEX_COORDS = 2
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

	void			LoadModel(const char* file_path);

	void			OnResize(int width, int height);

	const char*		GetDrivers() const;											// 
	bool			GetVsync() const;											// 
	void			SetVsync(bool set_to);										// 

	bool			GetGLFlag(GLenum cap) const;								// 
	bool			GetGLFlag(RENDERER_FLAGS cap) const;						// 
	void			SetGLFlag(GLenum cap, bool set_to);							// 
	void			SetGLFlag(RENDERER_FLAGS cap, bool set_to);					// 

	bool			GetGLShowWireframe() const;									//
	bool			GetGLShowNormals() const;									// 								
	bool			GetGLShowColors() const;									// 
	bool			GetGLShowTexCoords() const;									// 
	void			SetGLShowWireframe(bool set_to);							//
	void			SetGLShowNormals(bool set_to);								// 
	void			SetGLShowColors(bool set_to);								// 
	void			SetGLShowTexCoords(bool set_to);							// 

	void			PrimitiveExamples();										// REMOVE LATER

public:
	Light					lights[MAX_LIGHTS];									// 
	SDL_GLContext			context;											// 
	mat3x3					NormalMatrix;										// 
	mat4x4					ProjectionMatrix;									// 

	bool					vsync;												// Will keep track of whether or not the vsync is currently active.

	bool					gl_show_wireframe;									//
	bool					gl_show_normals;									// 
	bool					gl_show_colors;										// 
	bool					gl_show_tex_coords;									// 

	//std::vector<R_Mesh*>	meshes;
	std::vector<R_Model*>	models;

	PrimitiveDrawExamples	OGL_draw_examples;									// REMOVE LATER
	Cube					cube_direct;										// 
	Cube					cube_array;											// 
	Cube					cube_indices;										// 
	//Sphere				sphere;												// 								
	Pyramid					pyramid;											// 								
	//Cylinder				cylinder;											// ------------
};

#endif // !__M_RENDERER_3D_H__
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

	const char*		GetDrivers() const;
	bool			GetVsync() const;
	void			SetVsync(bool set_to);

	bool			GetGLDepthTest() const;
	bool			GetGLCullFace() const;
	bool			GetGLLighting() const;
	bool			GetGLColorMaterial() const;
	bool			GetGLTexture2D() const;
	bool			GetGLShowNormals() const;
	bool			GetGLShowColors() const;
	bool			GetGLShowTexCoords() const;

	void			SetGLDepthTest(bool set_to);
	void			SetGLCullFace(bool set_to);
	void			SetGLLighting(bool set_to);
	void			SetGLColorMaterial(bool set_to);
	void			SetGLTexture2D(bool set_to);
	void			SetGLShowNormals(bool set_to);
	void			SetGLShowColors(bool set_to);
	void			SetGLShowTexCoords(bool set_to);

	void			PrimitiveExamples();										// REMOVE LATER

public:
	Light					lights[MAX_LIGHTS];									// 
	SDL_GLContext			context;											// 
	mat3x3					NormalMatrix;										// 
	mat4x4					ProjectionMatrix;									// 

	bool					vsync;												// Will keep track of whether or not the vsync is currently active.

	bool					gl_depth_test;										// 
	bool					gl_cull_face;										// 
	bool					gl_lighting;										// 
	bool					gl_color_material;									// 
	bool					gl_texture_2D;										// 
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
#ifndef __M_RENDERER_3D_H__
#define __M_RENDERER_3D_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#include "Primitive.h"														// REMOVE LATER

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

	bool Init(Configuration& config) override;
	UPDATE_STATUS PreUpdate(float dt) override;
	UPDATE_STATUS PostUpdate(float dt) override;
	bool CleanUp() override;
	
public:
	void OnResize(int width, int height);

	void LoadModel(const char* file_path);

	void PrimitiveExamples();												// REMOVE LATER

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ProjectionMatrix;

	//std::vector<R_Mesh*> meshes;
	std::vector<R_Model*> models;

	PrimitiveDrawExamples OGL_draw_examples;								// REMOVE LATER
	Cube cube_direct;
	Cube cube_array;
	Cube cube_indices;
	//Sphere sphere;															
	Pyramid pyramid;														
	//Cylinder cylinder;														// ------------
};

#endif // !__M_RENDERER_3D_H__
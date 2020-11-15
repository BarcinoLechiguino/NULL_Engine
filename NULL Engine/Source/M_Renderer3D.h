#ifndef __M_RENDERER_3D_H__
#define __M_RENDERER_3D_H__

#include "MathStructures.h"
#include "glmath.h"

#include "Module.h"
#include "Globals.h"
#include "Light.h"

#define MAX_LIGHTS 8

class Configuration;
class R_Mesh;
class R_Model;
class GameObject;
class Primitive;

struct aiScene;

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

public:																		// --- RENDER GEOMETRY
	void			GenerateBuffers(R_Mesh* mesh);
	void			RenderMesh(float4x4 transform, R_Mesh* mesh, uint texture_id = 0, bool tex_is_active = true);
	//void			RenderGameObject(GameObject* game_object);
	//void			LoadModel(const char* file_path, vec4 mat_colour = vec4(1.0f, 1.0f, 1.0f, 1.0f));

	void			LoadDebugTexture();

	void			AddPrimitive(Primitive* primitive);
	void			CreatePrimitiveExamples();

public:																		// --- GET/SET METHODS
	//float4x4		GetProjectionMatrix() const;
	//float3x3		GetNormalMatrix() const;

	mat4x4			GetProjectionMatrix() const;
	mat3x3			GetNormalMatrix() const;
	
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
	bool			GetDrawPrimitiveExamples() const;						// 
	
	void			SetDrawWorldGrid(bool set_to);							// 
	void			SetDrawWorldAxis(bool set_to);							// 
	void			SetShowWireframe(bool set_to);							//
	void			SetShowNormals(bool set_to);							// 
	void			SetShowTexCoords(bool set_to);							// 
	void			SetDrawPrimtiveExamples(bool set_to);					//

public:
	Light					lights[MAX_LIGHTS];								// 
	SDL_GLContext			context;										// 

	uint					debug_texture_id;

	std::vector<Primitive*>	primitives;

private:
	//float4x4				projection_matrix;								//  
	//float3x3				normal_matrix;									// 

	mat4x4					projection_matrix;								// 
	mat3x3					normal_matrix;									// 

	bool					vsync;											// Will keep track of whether or not the vsync is currently active.

	bool					draw_world_grid;								//
	bool					draw_world_axis;								//
	bool					show_wireframe;									//
	bool					show_normals;									// 
	bool					show_tex_coords;								// 

	bool					draw_primitive_examples;
};

#endif // !__M_RENDERER_3D_H__
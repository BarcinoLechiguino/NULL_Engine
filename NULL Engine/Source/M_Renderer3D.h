#ifndef __M_RENDERER_3D_H__
#define __M_RENDERER_3D_H__

#include "MathGeoTransform.h"
#include "MathGeoLib/include/Geometry/LineSegment.h"
#include "MathGeoLib/include/Geometry/Triangle.h"

#include "Module.h"
#include "Light.h"

struct Color;
class ParsonNode;

class R_Model;
class R_Mesh;
class R_Material;
class R_Texture;

class C_Mesh;	
class C_Material;

class Primitive;

typedef unsigned int GLenum;

enum class RENDERER_FLAGS																						// Employed to avoid having OpenGL deps. in some files (M_Editor & "E_" files)
{
	DEPTH_TEST		= 0x0B71,																					// 0x0B71 = GL_DEPTH_TEST
	CULL_FACE		= 0x0B44,																					// 0x0B44 = GL_CULL_FACE
	LIGHTING		= 0x0B50,																					// 0x0B50 = GL_LIGHTING
	COLOR_MATERIAL	= 0x0B57,																					// 0x0B57 = GL_COLOR_MATERIAL
	TEXTURE_2D		= 0x0DE1,																					// 0x0DE1 = GL_TEXTURE_2D
	ALPHA_TEST		= 0x0BC0,																					// 0x0BC0 = GL_ALPHA_TEST
	BLEND			= 0x0BE2																					// 0x0BE2 = GL_BLEND
};

struct MeshRenderer
{
	MeshRenderer(const float4x4& transform, C_Mesh* c_mesh, C_Material* c_material);							// Will render the given mesh at the given position with the given mat & tex.

	void Render						();

	void RenderVertexNormals		(const R_Mesh* r_mesh);
	void RenderFaceNormals			(const R_Mesh* r_mesh);

	void GetFaces					(const R_Mesh* r_mesh, std::vector<Triangle>& vertex_faces, std::vector<Triangle>& normal_faces);

	void ApplyDebugParameters		();
	void ClearDebugParameters		();
	
	void ApplyTextureAndMaterial	();
	void ClearTextureAndMaterial	();

	float4x4	transform;
	C_Mesh*		c_mesh;
	C_Material*	c_material;
};

struct CuboidRenderer																							// Will render the wireframe of any given geometric form with 8 vertices.
{	
	CuboidRenderer(const float3* vertices, const Color& color);

	void Render();

	const float3*	vertices;
	const Color		color;
};

struct RayRenderer
{
	RayRenderer(const LineSegment& ray, const Color& color);

	void Render();

	const LineSegment	ray;
	const Color			color;
};

#define MAX_LIGHTS 8

class M_Renderer3D : public Module
{
public:
	M_Renderer3D(bool is_active = true);
	~M_Renderer3D();

	bool			Init				(ParsonNode& configuration) override;
	UPDATE_STATUS	PreUpdate			(float dt) override;
	UPDATE_STATUS	PostUpdate			(float dt) override;
	bool			CleanUp				() override;
	
	bool			LoadConfiguration	(ParsonNode& root) override;
	bool			SaveConfiguration	(ParsonNode& root) const override;

public:
	bool			InitOpenGL					();
	bool			InitGlew					();
	void			OnResize					();
	void			RecalculateProjectionMatrix	();

	void			InitFramebuffers			();
	void			LoadDebugTexture			();
	void			FreeBuffers					();

	void			RendererShortcuts			();

	void			RenderScene					();

public:																											// --- RENDER GEOMETRY
	void			GenerateBuffers				(const R_Mesh* mesh);
		
	void			DrawWorldGrid				(const int& size);
	void			DrawWorldAxis				();

	void			AddRenderersBatch			(const std::vector<MeshRenderer>& mesh_renderers, const std::vector<CuboidRenderer>& cuboid_renderers);
	void			RenderMeshes				();
	void			RenderCuboids				();
	void			RenderRays					();
	void			DeleteFromMeshRenderers		(C_Mesh* c_mesh_to_delete);
	void			DeleteFromCuboids			(float3* cuboid_to_delete);
	void			ClearRenderers				();																// Loading scene measure.

	void			AddPrimitive				(Primitive* primitive);
	void			CreatePrimitiveExamples		();

public:																											// --- GET/SET METHODS
	uint			GetDebugTextureID			() const;
	uint			GetSceneFramebuffer			() const;
	uint			GetSceneRenderTexture		() const;
	uint			GetGameFramebuffer			() const;

	uint			GetDepthBuffer				() const;
	uint			GetDepthBufferTexture		() const;

	const char*		GetDrivers					() const;														// 
	bool			GetVsync					() const;														// 
	void			SetVsync					(const bool& set_to);											// 

	bool			GetGLFlag					(GLenum flag) const;											// 
	bool			GetGLFlag					(RENDERER_FLAGS flag) const;									// 
	void			SetGLFlag					(GLenum flag, bool set_to);										// 
	void			SetGLFlag					(RENDERER_FLAGS flag, bool set_to);								// 

	bool			GetRenderWorldGrid			() const;														// 
	bool			GetRenderWorldAxis			() const;														//
	bool			GetRenderWireframes			() const;														//
	bool			GetRenderVertexNormals		() const;														// 
	bool			GetRenderFaceNormals		() const;														// 
	bool			GetRenderPrimitiveExamples	() const;														// 
	
	void			SetRenderWorldGrid			(const bool& set_to);											// 
	void			SetRenderWorldAxis			(const bool& set_to);											// 
	void			SetRenderWireframes			(const bool& set_to);											// 
	void			SetRenderVertexNormals		(const bool& set_to);											// 
	void			SetRenderFaceNormals		(const bool& set_to);											// 
	void			SetRenderPrimtiveExamples	(const bool& set_to);											// 

public:
	Light					lights[MAX_LIGHTS];																	// 
	SDL_GLContext			context;																			// 

	std::vector<Primitive*>	primitives;

private:
	std::vector<MeshRenderer>	mesh_renderers;
	std::vector<CuboidRenderer> cuboid_renderers;

	uint					scene_framebuffer;
	uint					depth_buffer;
	uint					scene_render_texture;
	uint					depth_buffer_texture;
	uint					game_framebuffer;
	uint					debug_texture_id;

	bool					vsync;																				// Will keep track of whether or not the vsync is currently active.

	bool					render_world_grid;																	// 
	bool					render_world_axis;																	// 
	bool					render_wireframes;																	//
	bool					render_vertex_normals;																// 
	bool					render_face_normals;																// 

	bool					render_primitive_examples;															//
};

#endif // !__M_RENDERER_3D_H__
#ifndef __R_MESH_H__
#define __R_MESH_H__

#include "MathGeoBoundingBox.h"
#include "MathGeoLib/include/Math/float2.h"
#include "Resource.h"

typedef unsigned __int32 uint;

struct Vertex
{
	float3 position;
	float3 normals;
	float2 tex_coords;
};

class R_Mesh : public Resource
{
public:
	R_Mesh();
	~R_Mesh();

	bool CleanUp() override;

public:
	void DrawVertexNormals();
	void DrawFaceNormals();

	void LoadBuffers();

	AABB GetAABB();
	void SetAABB();

public:
	//std::vector<Vertex>			vertices;
	std::vector<float>			vertices;
	std::vector<float>			normals;
	std::vector<float>			tex_coords;
	std::vector<uint>			indices;

	//std::vector<std::string>	tex_paths;
	//float4						colour;

	// Buffer data
	uint						VBO;					// Vertex Buffer Object.	-->		Will store all the buffer data of the vertices of the mesh.
	uint						NBO;					// Normals Buffer Object.	-->		Will store all the buffer data of the normals of the mesh.
	uint						TBO;					// Tex Coord Buffer Object. -->		Will store all the buffer data of the tex coords of the mesh.
	uint						IBO;					// Index Buffer Object.		--> 	Will store all the buffer data of the indices of the mesh.

	bool						draw_vertex_normals;
	bool						draw_face_normals;

	AABB						aabb;

private:

};

#endif // !__R_MESH_H__

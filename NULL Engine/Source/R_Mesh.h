#ifndef __R_MESH_H__
#define __R_MESH_H__

#include "MathGeoLib/src/Geometry/AABB.h"
#include "Resource.h"

enum class TEXTURE_TYPE									// The enum values are set according to the values of Assimp's aiTextureType enum.
{
	NONE		= 0x0,
	DIFFUSE		= 0x1,
	SPECULAR	= 0x2,
	UNKNOWN		= 0xC
};

struct Vertex
{
	Vertex();

	vec3 position;										// Position vector of the vertex in space.	Ex: (2.0f, 1.0f, -1.5f)		(x, y, z)
	vec3 normal;										// Normal vector of the vertex.				Ex: (0.7f, 0.0f, 0.7f)		(x, y, z)
	vec2 tex_coords;									// Texture coordinates of the vertex.		Ex:	(0.5f, 1.0f)			(u, v)			From 0 to 1.
};

struct Texture
{
	uint			id;									// Id of the texture.
	TEXTURE_TYPE	type;								// Diffuse or Specular.
	std::string		path;								// Path of the file in the directory. Will be used to avoid making duplicates.
};

class R_Mesh : public Resource
{
public:
	R_Mesh();
	~R_Mesh();

	void Draw(vec4 colour);
	void DrawNormals();
	void DrawTexCoords();

	void LoadDebugTexture();

	void LoadBuffers();
	void CreateAABB();									// Will create the bounding box that will encase the mesh.

public:
	// Mesh data
	std::vector<Vertex>		vertices;
	std::vector<uint>		indices;
	std::vector<Texture>	textures;

	vec4 colour;

	AABB aabb;											// Bounding box of the mesh.

private:
	// Buffer data
	uint VAO;											// Vertex Array Object.		-->		Will store all the buffer objects of the mesh.
	uint VBO;											// Vertex Buffer Object.	-->		Will store all the buffer data of the vertices of the mesh.
	uint IBO;											// Index Buffer Object.		--> 	Will store all the buffer data of the indices of the mesh.

	uint debug_texture_id;
};

#endif // !__R_MESH_H__
#ifndef __R_MESH_H__
#define __R_MESH_H__

#include "Resource.h"

class R_Mesh : public Resource
{
public:
	R_Mesh();
	~R_Mesh();

	void Draw(vec4 colour);
	void DrawNormals();

	void LoadBuffers();

	bool CleanUp();

public:
	std::vector<float>			vertices;
	std::vector<float>			normals;
	std::vector<float>			tex_coords;
	std::vector<uint>			indices;

	std::vector<std::string>	tex_paths;

	vec4						colour;

	// Buffer data
	uint						VBO;					// Vertex Buffer Object.	-->		Will store all the buffer data of the vertices of the mesh.
	uint						NBO;					// Normals Buffer Object.	-->		Will store all the buffer data of the normals of the mesh.
	uint						TBO;					// Tex Coord Buffer Object. -->		Will store all the buffer data of the tex coords of the mesh.
	uint						IBO;					// Index Buffer Object.		--> 	Will store all the buffer data of the indices of the mesh.

	bool						draw_normals;
	std::string					path;

	// Add a transform here?


private:

};

#endif // !__R_MESH_H__

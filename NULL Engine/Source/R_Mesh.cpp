#include "OpenGL.h"

#include "VariableTypedefs.h"

#include "R_Mesh.h"

#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64

R_Mesh::R_Mesh() : Resource(RESOURCE_TYPE::MESH), 
VBO					(0),																								// Initializing the buffers.
NBO					(0), 																								// 
TBO					(0), 																								//
IBO					(0), 																								// -------------------------
draw_vertex_normals	(false), 
draw_face_normals	(false)
{

}

R_Mesh::~R_Mesh()
{

}

bool R_Mesh::CleanUp()
{
	bool ret = true;

	// --- Delete Buffers
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &NBO);
	glDeleteBuffers(1, &TBO);
	glDeleteBuffers(1, &IBO);

	// --- Clear vectors
	vertices.clear();
	normals.clear();
	tex_coords.clear();
	indices.clear();

	return true;
}

// --- R_MESH METHODS
void R_Mesh::DrawVertexNormals()
{
	glBegin(GL_LINES);

	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);

	for (uint i = 0; i < vertices.size(); i += 3)																
	{
		glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
		glVertex3f(vertices[i] + normals[i], vertices[i + 1] + normals[i + 1], vertices[i + 2] + normals[i + 2]);
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnd();
}

void R_Mesh::DrawFaceNormals()
{
	glBegin(GL_LINES);

	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);

	/*for (uint i = 0; i < vertices.size(); i += 9)												// FIX THIS: Face normals are not displayed correctly.
	{
		if ((i + 8) < vertices.size())
		{
			float3 vertex_A = float3(vertices[i], vertices[i + 1], vertices[i + 2]);
			float3 vertex_B = float3(vertices[i + 3], vertices[i + 4], vertices[i + 5]);
			float3 vertex_C = float3(vertices[i + 6], vertices[i + 7], vertices[i + 8]);

			float3 normal_A = float3(vertices[i] + normals[i], vertices[i + 1] + normals[i + 1], vertices[i + 2] + normals[i + 2]);
			float3 normal_B = float3(vertices[i + 3] + normals[i + 3], vertices[i + 4] + normals[i + 4], vertices[i + 5] + normals[i + 5]);
			float3 normal_C = float3(vertices[i + 6] + normals[i + 6], vertices[i + 7] + normals[i + 7], vertices[i + 8] + normals[i + 8]);

			float3 vertex_avg = (vertex_A + vertex_B + vertex_C) * 0.333f;
			float3 normal_avg = (normal_A + normal_B + normal_C) * 0.333f;

			glVertex3f(vertex_avg.x, vertex_avg.y, vertex_avg.z);
			glVertex3f(normal_avg.x, normal_avg.y, normal_avg.z);
		}
	}*/

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnd();
}

void R_Mesh::LoadBuffers()
{
	if (!vertices.empty())
	{
		glGenBuffers(1, (GLuint*)&VBO);																			// Generates the Vertex Buffer Object
		glBindBuffer(GL_ARRAY_BUFFER, VBO);																		// Binds VBO with the GL_ARRAY_BUFFER biding point (target):
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);			// Inits the data stored inside VBO and specifies how the data will be accessed.
	}

	if (!normals.empty())
	{
		glGenBuffers(1, (GLuint*)&NBO);
		glBindBuffer(GL_ARRAY_BUFFER, NBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size(), &normals[0], GL_STATIC_DRAW);
	}

	if (!tex_coords.empty())
	{
		glGenBuffers(1, (GLuint*)&TBO);
		glBindBuffer(GL_ARRAY_BUFFER, TBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tex_coords.size(), &tex_coords[0], GL_STATIC_DRAW);
	}

	if (!indices.empty())
	{
		glGenBuffers(1, (GLuint*)&IBO);																			// Generates the Index Buffer Object
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);																// Binds IBO with the GL_ARRAY_BUFFER biding point (target):
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), &indices[0], GL_STATIC_DRAW);		// Inits the data stored inside IBO and specifies how the data will be accessed.
	}
}

AABB R_Mesh::GetAABB()
{
	return aabb;
}

void R_Mesh::SetAABB()
{
	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)&vertices[0], vertices.size() / 3);
}
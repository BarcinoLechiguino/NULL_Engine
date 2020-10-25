#include "MathGeoLib/src/MathGeoLib.h"
#include "OpenGL.h"

#include "R_Mesh.h"

R_Mesh::R_Mesh() : Resource()
{
	VAO = 0;																								// Initializing the buffers.
	VBO = 0;																								// 
	NBO = 0;																								// 
	TBO = 0;																								// 
	IBO = 0;																								// -----------------------------------
}

R_Mesh::~R_Mesh()
{

}

void R_Mesh::Draw(vec4 colour)
{	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(VAO);
	glColor4f(colour.r, colour.g, colour.b, colour.a);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

void R_Mesh::DrawNormals()
{
	/*std::vector<float> normals;
	
	for (uint i = 0; i < vertices.size(); ++i)
	{
		normals.push_back(vertices[i].position.x);
		normals.push_back(vertices[i].position.y);
		normals.push_back(vertices[i].position.z);

		normals.push_back(vertices[i].position.x + vertices[i].normal.x);
		normals.push_back(vertices[i].position.y + vertices[i].normal.y);
		normals.push_back(vertices[i].position.z + vertices[i].normal.z);
	}*/

	glBegin(GL_LINES);

	for (uint i = 0; i < vertices.size(); ++i)																// Each vertex will have the shared normals of 3 different faces.
	{
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
		glVertex3f(vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
		glVertex3f(vertices[i].position.x + vertices[i].normal.x, vertices[i].position.y + vertices[i].normal.y, vertices[i].position.z + vertices[i].normal.z);
	}

	glEnd();
}

void R_Mesh::DrawTexCoords()
{
	glBegin(GL_POINTS);

	glPointSize(100.0f);

	for (uint i = 0; i < vertices.size(); ++i)
	{
		glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
		glVertex3f(vertices[i].position.x + vertices[i].tex_coords.x, vertices[i].position.y + vertices[i].tex_coords.y, vertices[i].position.z);
	}

	glPointSize(1.0f);

	glEnd();
}

void R_Mesh::LoadBuffers()
{
	glGenVertexArrays(1, (GLuint*)&VAO);																	// Generates the Vertex Array Object.
	glBindVertexArray(VAO);																					// Binds VAO.

	glGenBuffers(1, (GLuint*)&VBO);																			// Generates the Vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);																		// Binds VBO with the GL_ARRAY_BUFFER biding point (target):
 	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);			// Inits the data stored inside VBO and specifies how the data will be accessed.

	glGenBuffers(1, (GLuint*)&IBO);																			// Generates the Index Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);																// Binds IBO with the GL_ARRAY_BUFFER biding point (target):
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), &indices[0], GL_STATIC_DRAW);		// Inits the data stored inside IBO and specifies how the data will be accessed.

	//Vertices
	glEnableVertexAttribArray(0);																			// Uses the currently bound VAO to enable the specified generic vertex attrib array.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);								// Defines an array of generic vertex attribute data. (Size, type...)

	// Normals
	glEnableVertexAttribArray(1);																			// Uses the currently bound VAO to enable the specified generic vertex attrib array.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));		// offsetof() sets the byte offsets of: normal vec offset = normal attribute offset.

	// Texture coordinates
	glEnableVertexAttribArray(2);																			// Uses the currently bound VAO to enable the specified generic vertex attrib array.
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));	// The byte offset of the tex_coord vector = byte offset tex_coord attrib offset.

	glBindVertexArray(0);																					// Breaks the existing VAO binding.
}

void R_Mesh::CreateAABB()
{
	aabb.SetNegativeInfinity();
	aabb.Enclose((math::float3*)&vertices[0], vertices.size());
}

// VERTEX METHODS
Vertex::Vertex()
{
	position	= { 0.0f, 0.0f, 0.0f };
	normal		= { 0.0f, 0.0f, 0.0f };
	tex_coords	= { 0.0f, 0.0f };
}
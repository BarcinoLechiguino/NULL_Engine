#include "MathGeoLib/src/MathGeoLib.h"
#include "OpenGL.h"

#include "R_Mesh.h"

R_Mesh::R_Mesh() : Resource()
{
	VAO = 0;																								// Initializing the buffers.
	VBO = 0;																								// 
	IBO = 0;																								// -----------------------------------

	debug_texture_id = 0;

	LoadDebugTexture();
}

R_Mesh::~R_Mesh()
{

}

void R_Mesh::Draw(vec4 colour)
{	
	for (uint i = 0; i < textures.size(); ++i)
	{
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glBindTexture(GL_TEXTURE_2D, debug_texture_id);

	glBindVertexArray(VAO);

	glColor4f(colour.r, colour.g, colour.b, colour.a);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void R_Mesh::DrawNormals()
{
	glBegin(GL_LINES);

	for (uint i = 0; i < vertices.size(); ++i)																
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

	for (uint i = 0; i < vertices.size(); ++i)
	{
		glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
		glVertex3f(vertices[i].position.x + vertices[i].tex_coords.x, vertices[i].position.y + vertices[i].tex_coords.y, vertices[i].position.z);
	}

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
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));	// The byte offset of the tex_coord vector = byte offset tex_coord attrib offset.

	glBindVertexArray(0);																					// Breaks the existing VAO binding.
}

void R_Mesh::LoadDebugTexture()
{
	GLubyte checker_image[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];					// HEIGHT columns, WIDTH rows and 4 variables per checker (for RGBA purposes).

	for (int i = 0; i < CHECKERS_HEIGHT; ++i)									// There will be CHECKERS_WIDTH rows per column.
	{
		for (int j = 0; j < CHECKERS_WIDTH; ++j)								// There will be an RGBA value per checker.
		{
			int color = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;			// Getting whether the checker will be white or black according to the iteration indexes and bitwise operations.

			checker_image[i][j][0] = (GLubyte)color;							// R
			checker_image[i][j][1] = (GLubyte)color;							// G
			checker_image[i][j][2] = (GLubyte)color;							// B
			checker_image[i][j][3] = (GLubyte)255;								// A

			//LOG("CHECKER COLOR: %s", color == 255 ? "White" : "Black");
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);										// Sets the pixel storage modes. GL_UNPACK_ALIGNMENT specifies the alignment requirements for the
	// --->																		// start of each pixel row in memory. 1 means that the alignment requirements will be byte-alignment.
	glGenTextures(1, &debug_texture_id);										// Generate texture names. Returns n names in the given buffer. GL_INVALID_VALUE if n is negative.
	glBindTexture(GL_TEXTURE_2D, debug_texture_id);								// Bind a named texture to a texturing target. Binds the buffer with the given target (GL_TEXTURE_2D).
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);				// Set texture parameters. WRAP_S: Set the wrap parameters for texture coordinate s. GL_REPEAT is the default.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);				// WRAP_T: Set the wrap parameters for the texture coordinate r
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);			// MAG_FILTER: Sets the texture magnification process parameters. GL_NEAREST rets the val. of nearest tex elem.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);			// MIN_FILTER: Sets the texture minimization process parameters. " ".
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	// MIPMAP_NEAREST: Same as GL_NEAREST but works with the mipmaps generated by glGenerateMipmap() to
	// --->																				// handle the process of resizing a tex. Takes the mipmap that most closely matches the size of the px.

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checker_image);

	//glGenerateMipmap(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, 0);
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
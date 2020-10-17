/*#include "Dependencies/glew/include/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "Source/Dependencies/glew/include/glew.h");*/

#include "OpenGL.h"
#include "Globals.h"
#include "Primitive.h"
#include "Application.h"

// ------------------------------------------------------------
Primitive::Primitive() : transform(IdentityMatrix), color(White), wire(false), axis(false), type(PRIMITIVE_TYPES::P_POINT)
{}

// ------------------------------------------------------------
PRIMITIVE_TYPES Primitive::GetType() const
{
	return type;
}

void Primitive::Update()
{

}

// ------------------------------------------------------------
void Primitive::Render() const
{
	glPushMatrix();
	glMultMatrixf(transform.M);

	if (axis == true)
	{
		// Draw Axis Grid
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);											// X Axis.
		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
		glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);											// Y Axis.
		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);											// Z Axis.
		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
		glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
		glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

		glEnd();

		glLineWidth(1.0f);
	}


	glColor3f(color.r, color.g, color.b);

	if (wire || App->renderPrimitives == false)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	InnerRender();

	glPopMatrix();
}

// ------------------------------------------------------------
void Primitive::InnerRender() const
{
	glPointSize(5.0f);

	glBegin(GL_POINTS);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();

	glPointSize(1.0f);
}

// ------------------------------------------------------------
void Primitive::SetPos(float x, float y, float z)
{
	transform.translate(x, y, z);
}

vec3 Primitive::GetPos() const
{
	vec3 position = { 0.0f, 0.0f, 0.0f };

	position.x = transform.M[12];				// In a 4x4 matrix, the top-left 3x3 matrix is the rotation and the left column's first 3 elements are the position.
	position.y = transform.M[13];
	position.z = transform.M[14];

	return position;
}

// ------------------------------------------------------------
void Primitive::SetRotation(float angle, const vec3 &u)
{
	transform.rotate(angle, u);
}

// ------------------------------------------------------------
void Primitive::Scale(float x, float y, float z)
{
	transform.scale(x, y, z);
}

// CUBE ============================================

Cube::Cube(const vec3& _size, float mass) : Primitive(), size(_size), loaded_in_array(false), loaded_in_indices(false)
{
	type = PRIMITIVE_TYPES::CUBE;
}

vec3 Cube::GetSize() const
{
	return size;
}

void Cube::DirectRender() const
{	
	float sx = size.x * 0.5f;
	float sy = size.y * 0.5f;
	float sz = size.z * 0.5f;

	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-sx, -sy, sz);
	glVertex3f( sx, -sy, sz);
	glVertex3f( sx,  sy, sz);
	glVertex3f(-sx,  sy, sz);

	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f( sx, -sy, -sz);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f(-sx,  sy, -sz);
	glVertex3f( sx,  sy, -sz);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(sx, -sy,  sz);
	glVertex3f(sx, -sy, -sz);
	glVertex3f(sx,  sy, -sz);
	glVertex3f(sx,  sy,  sz);

	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f(-sx, -sy,  sz);
	glVertex3f(-sx,  sy,  sz);
	glVertex3f(-sx,  sy, -sz);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-sx, sy,  sz);
	glVertex3f( sx, sy,  sz);
	glVertex3f( sx, sy, -sz);
	glVertex3f(-sx, sy, -sz);

	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f( sx, -sy, -sz);
	glVertex3f( sx, -sy,  sz);
	glVertex3f(-sx, -sy,  sz);

	glEnd();
}

void Cube::ArrayRender()
{	
	if (!loaded_in_array)
	{
		uint my_id = 0;

		const uint array_size = 108;										// 6 faces with 2 triangles per face --> 12 vertex with 3 coordinates and 3 vertices. TOTAL: 6 * 2 * 3 * 3 = 108

		float cube_vertex_coords[array_size] =								// Being ABCDEFGH all the vertex in a Cube. Starting at the front bottom left and ending at the back top right.
		{
			// --- (0.0f, 0.0f, 0.0f) AS THE CENTER
			// -- FRONT --
			-1.0f, -1.0f, 1.0f,		//ABC
			1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			
			1.0f, -1.0f, 1.0f,		//BDC
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			
			// -- RIGHT --
			1.0f, -1.0f, 1.0f,		//BFD
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			
			1.0f, -1.0f, -1.0f,		//FHD
			1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			
			// -- BACK --
			-1.0f, -1.0f, -1.0f,	//EGH
			-1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			
			1.0f, -1.0f, -1.0f,		//FEH
			-1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			
			// -- LEFT --
			-1.0f, -1.0f, -1.0f,	//EAG
			-1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,
			
			-1.0f, -1.0f, 1.0f,		//ACG
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,
			
			// -- BOTTOM --
			-1.0f, -1.0f, 1.0f,		//AEF
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			
			-1.0f, -1.0f, 1.0f,		//AFB
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, 1.0f,
			
			// -- TOP --
			-1.0f, 1.0f, 1.0f,		//CDG
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,
			
			1.0f, 1.0f, 1.0f,		//DHG
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f
			// ------------------------------------
		};

		int j = 0;
		for (uint i = 0; i < array_size; ++i)					// Resizing the cube according to the size parameter.
		{
			if (j == 0)
			{
				//cube_vertex_coords[i] *= size.x;
				cube_vertex_coords[i] *= size.x * 0.5f;
				j++;
			}
			else if (j == 1)
			{
				//cube_vertex_coords[i] *= size.y;
				cube_vertex_coords[i] *= size.y * 0.5f;
				j++;
			}
			else if (j == 2)
			{
				//cube_vertex_coords[i] *= size.z;
				cube_vertex_coords[i] *= size.z * 0.5f;
				j = 0;
			}
		}

		glGenBuffers(1, (GLuint*)&my_id);
		glBindBuffer(GL_ARRAY_BUFFER, my_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, cube_vertex_coords, GL_STATIC_DRAW);

		loaded_in_array = true;
	}

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, NULL);
	
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void Cube::IndicesRender()
{	
	if (!loaded_in_indices)
	{
		const uint array_size = 24;						// 8 vertex by 3 coordinates per vertex.
		
		float cube_vertex_coords[array_size] =			// Being ABCDEFGH all the vertex in a Cube. Starting at the front bottom left and ending at the back top right.
		{
			// --- (0.0f, 0.0f, 0.0f) AS THE CENTER
			-1.0f, -1.0f,  1.0f,						// A --- i = 0
			 1.0f, -1.0f,  1.0f,						// B --- i = 1
			-1.0f,  1.0f,  1.0f,						// C --- i = 2
			 1.0f,  1.0f,  1.0f,						// D --- i = 3
			-1.0f, -1.0f, -1.0f,						// E --- i = 4
			 1.0f, -1.0f, -1.0f,						// F --- i = 5
			-1.0f,  1.0f, -1.0f,						// G --- i = 6
			 1.0f,  1.0f, -1.0f							// H --- i = 7
		};

		ApplyTransform(cube_vertex_coords, array_size);
		ApplySize(cube_vertex_coords, array_size);

		uint indices[36] =
		{
			0, 1, 2,				//ABC
			1, 3, 2,				//BDC
			
			1, 5, 3,				//BFD
			5, 7, 3,				//FHD
			
			4, 6, 7,				//EGH
			5, 4, 7,				//FEH
			
			4, 0, 6,				//EAG
			0, 2, 6,				//ACG
			
			0, 4, 5,				//AEF
			1, 0, 5,				//BAF
			
			2, 3, 6,				//CDG
			3, 7, 6,				//DHG
		};

		uint my_id = 0;
		uint my_indices = 0;

		glGenBuffers(1, (GLuint*)&my_id);
		glBindBuffer(GL_ARRAY_BUFFER, my_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, cube_vertex_coords, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&my_indices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 36, indices, GL_STATIC_DRAW);

		loaded_in_indices = true;
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(transform.M[12], transform.M[13], transform.M[14]);
	//glRotatef(angle, x, y, z);
	//glScalef(x, y, z);

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}

void Cube::ApplyTransform(float* coordinates, int array_size)
{
	vec3 position = GetPos();

	for (int i = 0; i < array_size; ++i)
	{
		coordinates[i]		+= position.x;
		coordinates[++i]	+= position.y;
		coordinates[++i]	+= position.z;
	}
}

void Cube::ApplySize(float* coordinates, int array_size)
{
	for (uint i = 0; i < array_size; ++i)					// Resizing the cube according to the size parameter.
	{
		coordinates[i]		*= size.x * 0.5f;
		coordinates[++i]	*= size.y * 0.5f;
		coordinates[++i]	*= size.z * 0.5f;
	}
}

// SPHERE ============================================

/*Sphere::Sphere(float _radius, float mass) : Primitive(), radius(_radius), rings(1), sectors(1), loaded_buffers(false)
{
	type = PRIMITIVE_TYPES::SPHERE;
}*/

Sphere::Sphere(float radius, uint rings, uint sectors) : Primitive(), radius(radius), rings(rings), sectors(sectors), loaded_buffers(false)
{
	type = PRIMITIVE_TYPES::SPHERE;
}

float Sphere::GetRadius() const
{
	return radius;
}

uint Sphere::GetRings() const
{
	return rings;
}

uint Sphere::GetSectors() const
{
	return sectors;
}

void Sphere::SetRings(uint rings)
{
	this->rings = rings;

	loaded_buffers = false;
}

void Sphere::SetSectors(uint sectors)
{
	this->sectors = sectors;

	loaded_buffers = false;
}

void Sphere::IndiceRender()
{	
	if (!loaded_buffers)
	{
		float const ring_constant = 1.0f / (float)(rings - 1);
		float const sector_constant = 1.0f / (float)(sectors - 1);

		const uint vertices_size	= rings * sectors * 3;											// rings per sectors per ring per 3 coordinates per vertex. Total: rings * sectors * 3;
		const uint normals_size		= rings * sectors * 3;
		const uint tex_coords_size	= rings * sectors * 2;
		const uint indices_size		= rings * sectors * 4;

		vertices.resize(vertices_size);
		normals.resize(normals_size);
		tex_coords.resize(tex_coords_size);
		indices.resize(indices_size);

		std::vector<float>::iterator vert = vertices.begin();
		std::vector<float>::iterator norm = normals.begin();
		std::vector<float>::iterator texc = tex_coords.begin();
		
		int v = 0;
		for (int ring = 0; ring < rings; ++ring)
		{
			for (int sector = 0; sector < sectors; ++sector)
			{
				float const x = cos(2 * PI * sector * sector_constant) * sin(PI * ring * ring_constant);
				float const y = sin(-HALF_PI + PI * ring * ring_constant);
				float const z = sin(2 * PI * sector * sector_constant) * sin(PI * ring * ring_constant);

				*vert++ = x * radius;																// Calculating the position coordinates of each vertex
				*vert++ = y * radius;																// according to the axis, the ring and sector consts
				*vert++ = z * radius;																// and the radius.

				*norm++ = x;																		// Calculating the normal vector coordinates of each vertex
				*norm++ = y;																		//
				*norm++ = z;																		//

				*texc++ = sector * sector_constant;													//
				*texc++ = ring * ring_constant;														//
			}
		}

		std::vector<uint>::iterator ind = indices.begin();
		for (int ring = 0; ring < rings; ++ring)
		{
			for (int sector = 0; sector < sectors; ++sector)
			{
				*ind++ = ring * sectors + sector;
				*ind++ = ring * sectors + (sector + 1);
				*ind++ = (ring + 1) * sectors + (sector + 1);
				*ind++ = (ring + 1) * sectors + sector;
			}
		}

		uint my_vertices = 0;
		uint my_indices = 0;
		
		glGenBuffers(1, (GLuint*)&my_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, my_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&my_indices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), &indices[0], GL_STATIC_DRAW);

		loaded_buffers = true;
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(transform.M[12], transform.M[13], transform.M[14]);
	
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}

void Sphere::InnerRender() const
{
	//glutSolidSphere(radius, 25, 25);
}

// CYLINDER ============================================
Cylinder::Cylinder(float radius, float height, float mass) : Primitive(), radius(radius), height(height)
{
	type = PRIMITIVE_TYPES::CYLINDER;
}

float Cylinder::GetRadius() const
{
	return radius;
}

float Cylinder::GetHeight() const
{
	return height;
}

void Cylinder::InnerRender() const
{
	glPushMatrix();
	mat4x4 rotateMat = IdentityMatrix;
	rotateMat.rotate(90.f, vec3(0, 0, 1));
	glMultMatrixf(&rotateMat);

	int n = 30;

	// Cylinder Bottom
	glBegin(GL_POLYGON);
	
	for(int i = 360; i >= 0; i -= (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(-height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder Top
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	for(int i = 0; i <= 360; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder "Cover"
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i < 480; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians

		glVertex3f(height*0.5f,  radius * cos(a), radius * sin(a) );
		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a) );
	}
	glEnd();

	glPopMatrix();
}

// PYRAMID ============================================
Pyramid::Pyramid(vec3 size) : Primitive(), size(size), loaded_in_buffers(false)
{
	type = PRIMITIVE_TYPES::PYRAMID;
}

void Pyramid::IndiceRender()
{
	if (!loaded_in_buffers)
	{
		/*vertices.resize(15);
		indices.resize(18);

		// VERTICES ----------------
		vertices.add();
		// -------------------------

		// INDICES ----------------

		// ------------------------*/

		float vertices_ex[15] =
		{
			 0.0f,  1.015f,  0.0f,		// A --- i = 0		// Height of a given equilateral pyramid: (sqrt(6) / 3) * a; where a is the length of the sides of the base.
			-1.0f, -0.615f,  1.0f,		// B --- i = 1
			 1.0f, -0.615f,  1.0f,		// C --- i = 2
			-1.0f, -0.615f, -1.0f,		// D --- i = 3
			 1.0f, -0.615f, -1.0f		// E --- i = 4
		};

		uint indices_ex[18] =
		{
			0, 1, 2,					// ABC --- FRONT
			0, 2, 4,					// ACE --- RIGHT
			0, 4, 3,					// AED --- BACK
			0, 3, 1,					// ADB --- LEFT

			1, 3, 4,					// BDE --- BASE
			1, 4, 2						// BEC ---  -
		};

		uint my_vertices = 0;
		uint my_indices = 0;

		glGenBuffers(1, (GLuint*)&my_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, my_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 15, vertices_ex, GL_STATIC_DRAW);

		glGenBuffers(1, (GLuint*)&my_indices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 24, indices_ex, GL_STATIC_DRAW);
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(transform.M[12], transform.M[13], transform.M[14]);

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}

// LINE ==================================================
Line::Line() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
{
	type = PRIMITIVE_TYPES::LINE;
}

Line::Line(const vec3& A, const vec3& B) : Primitive(), origin(A), destination(B)
{
	type = PRIMITIVE_TYPES::LINE;
}

vec3 Line::GetOrigin() const
{
	return origin;
}

vec3 Line::GetDestination() const
{
	return destination;
}

void Line::InnerRender() const
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(destination.x, destination.y, destination.z);

	glEnd();

	glLineWidth(1.0f);
}

// PLANE ==================================================
Plane::Plane(const vec3& _normal) : Primitive(), normal(_normal)
{
	type = PRIMITIVE_TYPES::PLANE;
}

vec3 Plane::GetNormal() const
{
	return normal;
}

void Plane::InnerRender() const
{
	glLineWidth(1.0f);

	glBegin(GL_LINES);

	float d = 200.0f;

	for(float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();
}

PrimitiveDrawExamples::PrimitiveDrawExamples() : Primitive(), size(1.0f), origin(vec3(0.0f, 0.0f, 0.0f))
{

}

PrimitiveDrawExamples::PrimitiveDrawExamples(float size, vec3 origin) : Primitive(), size(size), origin(origin)
{	
	transform.translate(origin.x, origin.y, origin.z);
}

void PrimitiveDrawExamples::DrawAllExamples()
{
	GL_PointsExample();
	GL_LinesExample();
	GL_LineStripExample();
	GL_LineLoopExample();
	GL_PolygonExample();
	GL_TrianglesExample();
	GL_TriangleStripExample();
	GL_TriangleFanExample();
	GL_QuadsExample();
	GL_QuadStripExample();
}

void PrimitiveDrawExamples::GL_PointsExample(int index)
{
	float offset = 4.0f * index;
	
	glPointSize(4.0f);

	glBegin(GL_POINTS);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glVertex3f(0.0f + offset, 2.0f, 0.0f);						// A

	glVertex3f(1.5f + offset, 2.0f, 0.0f);						// B

	glVertex3f(2.0f + offset, 3.0f, 0.0f);						// C

	glVertex3f(1.5f + offset, 4.0f, 0.0f);						// D

	glVertex3f(0.0f + offset, 4.0f, 0.0f);						// E

	glVertex3f(-0.5f + offset, 3.0f, 0.0f);						// F


	glEnd();

	glPointSize(1.0f);
}

void PrimitiveDrawExamples::GL_LinesExample(int index)
{
	float offset = 4.0f * index;
	
	glLineWidth(2.0f);
	
	glBegin(GL_LINES);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glVertex3f(0.0f + offset, 2.0f, 0.0f);						// AB
	glVertex3f(1.5f + offset, 2.0f, 0.0f);

	glVertex3f(2.0f + offset, 3.0f, 0.0f);						// CD
	glVertex3f(1.5f + offset, 4.0f, 0.0f);

	glVertex3f(0.0f + offset, 4.0f, 0.0f);						// EF
	glVertex3f(-0.5f + offset, 3.0f, 0.0f);

	glEnd();

	glLineWidth(1.0f);
}

void PrimitiveDrawExamples::GL_LineStripExample(int index)
{
	float offset = 4.0f * index;

	glLineWidth(2.0f);

	glBegin(GL_LINE_STRIP);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glVertex3f(0.0f + offset, 2.0f, 0.0f);						// ABCDE --> Last segment is not drawn --> Still a line, not a polygon.
	glVertex3f(1.5f + offset, 2.0f, 0.0f);
	glVertex3f(2.0f + offset, 3.0f, 0.0f);
	glVertex3f(1.5f + offset, 4.0f, 0.0f);
	glVertex3f(0.0f + offset, 4.0f, 0.0f);
	glVertex3f(-0.5f + offset, 3.0f, 0.0f);

	glEnd();

	glLineWidth(1.0f);
}

void PrimitiveDrawExamples::GL_LineLoopExample(int index)
{
	float offset = 4.0f * index;

	glLineWidth(2.0f);

	glBegin(GL_LINE_LOOP);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glVertex3f(0.0f + offset, 2.0f, 0.0f);						// ABCDEF --> Closed loop --> Polygon but not considered as such by OpenGL (not filled).
	glVertex3f(1.5f + offset, 2.0f, 0.0f);
	glVertex3f(2.0f + offset, 3.0f, 0.0f);
	glVertex3f(1.5f + offset, 4.0f, 0.0f);
	glVertex3f(0.0f + offset, 4.0f, 0.0f);
	glVertex3f(-0.5f + offset, 3.0f, 0.0f);

	glEnd();

	glLineWidth(1.0f);
}

void PrimitiveDrawExamples::GL_PolygonExample(int index)
{
	float offset = 4.0f * index;

	glLineWidth(2.0f);

	glBegin(GL_POLYGON);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glVertex3f(0.0f + offset, 2.0f, 0.0f);						// ABCDEF --> Polygon and is considered as such by OpenGL (filled).
	glVertex3f(1.5f + offset, 2.0f, 0.0f);
	glVertex3f(2.0f + offset, 3.0f, 0.0f);
	glVertex3f(1.5f + offset, 4.0f, 0.0f);
	glVertex3f(0.0f + offset, 4.0f, 0.0f);
	glVertex3f(-0.5f + offset, 3.0f, 0.0f);

	glEnd();

	glLineWidth(1.0f);
}

void PrimitiveDrawExamples::GL_TrianglesExample(int index)
{
	float offset = 3.95f * index;

	glLineWidth(2.0f);

	glBegin(GL_TRIANGLES);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glVertex3f(0.0f + offset, 2.0f, 0.0f);						// ABC --> Same as GL_POLYGON but only for triangles.
	glVertex3f(3.0f + offset, 2.0f, 0.0f);
	glVertex3f(1.5f + offset, 4.0f, 0.0f);

	glEnd();

	glLineWidth(1.0f);
}

void PrimitiveDrawExamples::GL_TriangleStripExample(int index)
{
	float offset = 4.0f * index;

	glLineWidth(2.0f);
	
	glBegin(GL_TRIANGLE_STRIP);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glVertex3f(0.0f + offset, 4.0f, 0.0f);						// Vertex are positioned in a zig-zag and GL_TRIANGLE_STRIP closes the zig-zag in triangles.
	glVertex3f(1.5f + offset, 2.0f, 0.0f);
	glVertex3f(3.0f + offset, 4.0f, 0.0f);
	glVertex3f(4.5f + offset, 2.0f, 0.0f);
	glVertex3f(6.0f + offset, 4.0f, 0.0f);
	glVertex3f(7.5f + offset, 2.0f, 0.0f);
	glVertex3f(9.0f + offset, 4.0f, 0.0f);

	glEnd();

	glLineWidth(1.0f);

}

void PrimitiveDrawExamples::GL_TriangleFanExample(int index)
{
	float offset = 5.20f * index;

	glLineWidth(2.0f);

	glBegin(GL_TRIANGLE_FAN);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	//glVertex3f(0.0f, 3.0f, 0.0f);												// Center of the triangle fan.
	glVertex3f(offset, 3.0f, 0.0f);												// Center of the triangle fan.

	for (int i = 360 - 30; i >= 0 - 30; i -= (360 / 6))							// i = 360 - 30  so it is the same as the previously drawn hexagons.
	{	
		float a = i * M_PI / 180; // degrees to radians
		//glVertex3f(2.0f * sin(a), 2.0f * cos(a), 0.0f);						// Triangle fan centered at (0.0f, 0.0f, 0.0f)
		//glVertex3f((2.0f * sin(a)), (2.0f * cos(a)) + 3.0f, 0.0f);			// Triangle fan centered at (0.0f, 3.0f, 0.0f)
		glVertex3f((1.5f * sin(a)) + offset, (1.5f * cos(a)) + 3.0f, 0.0f);		// Triangle fan centered at (position_offset, 3.0f, 0.0f)
	}

	glEnd();

	glLineWidth(1.0f);
}

void PrimitiveDrawExamples::GL_QuadsExample(int index)
{
	float offset = 5.05f * index;

	glLineWidth(2.0f);

	glBegin(GL_QUADS);								// Same as GL_TRIANGLES, like GL_POLYGON but only for quads.

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	glVertex3f(0.0f + offset, 2.0f, 0.0f);			// ABCD
	glVertex3f(2.0f + offset, 2.0f, 0.0f);			// Declaration order has to be counter-clockwise (Right Hand Rule). Otherwise the poligon will be facing backwards (Backface culling).
	glVertex3f(2.0f + offset, 4.0f, 0.0f);
	glVertex3f(0.0f + offset, 4.0f, 0.0f);

	glEnd();

	glLineWidth(1.0f);
}

void PrimitiveDrawExamples::GL_QuadStripExample(int index)
{
	float offset = 5.0f * index;

	glLineWidth(2.0f);

	glBegin(GL_QUAD_STRIP);						// Very similar to GL_TRIANGLE_STRIP. The vertexs are declared in a zig-zag and they will be conected in quads.

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glVertex3f(0.0f + offset, 4.0f, 0.0f);
	glVertex3f(0.0f + offset, 2.0f, 0.0f);
	glVertex3f(2.0f + offset, 4.0f, 0.0f);
	glVertex3f(2.0f + offset, 2.0f, 0.0f);
	glVertex3f(4.0f + offset, 4.0f, 0.0f);
	glVertex3f(4.0f + offset, 2.0f, 0.0f);
	glVertex3f(6.0f + offset, 4.0f, 0.0f);
	glVertex3f(6.0f + offset, 2.0f, 0.0f);

	glEnd();

	glLineWidth(1.0f);
}
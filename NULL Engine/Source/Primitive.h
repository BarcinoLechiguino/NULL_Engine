#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include <vector>
#include <cmath>

#include "glmath.h"
#include "Color.h"

enum class PRIMITIVE_TYPES
{
	P_POINT,
	LINE,
	PLANE,
	CUBE,
	SPHERE,
	CYLINDER,
	PYRAMID,
	NONE
};

enum class BUFFER_TYPE
{
	VERTICES,
	NORMALS,
	UVS,
	INDICES,
	MAX_BUFFER_TYPES
};

class Primitive
{
public:
	Primitive();

	void Update();

	virtual void	Render() const;
	//virtual void	IndicesRender();
	void			LoadBuffersOnMemory();

public:
	void			SetPos(float x, float y, float z);
	vec3			GetPos() const;
	void			SetRotation(float angle, const vec3 &u);
	void			Scale(float x, float y, float z);
	PRIMITIVE_TYPES	GetType() const;

public:
	Color color;
	mat4x4 transform;
	bool axis, wire;

	uint VAO;																// Defines the Vertex Array Object that will hold all buffer objects of a primitive.
	uint buffers[(uint)BUFFER_TYPE::MAX_BUFFER_TYPES];						// Will hold all the possible buffers that a given primitive might use.
	uint bufferSize[(uint)BUFFER_TYPE::MAX_BUFFER_TYPES];					// Will be used to define the size in bytes of a given buffer.

	std::vector<float>	vertices;
	std::vector<float>	normals;
	std::vector<float>	uvs;
	std::vector<uint>	indices;

protected:
	virtual void InnerRender() const;

protected:
	PRIMITIVE_TYPES type;
};

// ============================================
class Cube : public Primitive
{
public :
	Cube(const vec3& size = vec3(1.f, 1.f, 1.f), float mass = 1.f);

	vec3 GetSize() const;

public:
	void DirectRender() const;
	void ArrayRender();
	void IndicesRender();

	void ApplyTransform(float* coordinates, int array_size);
	void ApplySize(float* coordinates, int array_size);

protected:
	//void DirectRender() const;
	//void ArrayRender();
	//void IndicesRender();

private:
	vec3 size;

	//unsigned int VAO;

	bool loaded_in_array;
	bool loaded_in_indices;
};

// ============================================
class Sphere : public Primitive
{
public:
	//Sphere(float radius = 1.f, float mass = 1.f);
	Sphere(float radius = 1.f, uint rings = 12, uint sectors = 24);

public:
	float GetRadius() const;
	uint GetRings() const;
	uint GetSectors() const;

	void SetRings(uint rings);
	void SetSectors(uint sectors);

	void IndiceRender();
	//void IndiceRender(float radius, const uint rings, const uint sectors);

protected:
	void InnerRender() const;

protected:
	bool loaded_buffers;

private:
	float radius;
	uint rings;
	uint sectors;
};

// ============================================
class Cylinder : public Primitive
{
public:
	Cylinder(float radius = 1.f, float height = 2.f, uint sectors = 6, float mass = 1.f);

	float GetRadius() const;
	float GetHeight() const;
	uint GetSectors() const;
	void SetSectors(uint sectors);

	vec3 GetPosition() const;
	void SetPosition(vec3 position);

	void InnerRender() const;
	
	std::vector<float> GetCircularVertices();
	void IndicesRender();

	void ConstructCoverVertices(std::vector<float> circular_vertices);
	void ConstructBaseAndTopVertices(std::vector<float> circular_vertices);
	void ConstructIndices(uint base_center_index, uint top_center_index);

protected:
	//void InnerRender() const;

private:
	float radius;
	float height;
	uint sectors;

	bool loaded_in_buffers;
};

// ============================================
class Pyramid : public Primitive
{
public:
	Pyramid(vec3 size = vec3(1.0f, 1.0f, 1.0f));

	void IndicesRender();

protected:
	std::vector<float> vertices;
	std::vector<uint> indices;

private:
	vec3 size;

	bool loaded_in_buffers;
};

// ============================================
class Line : public Primitive
{
public:
	Line();
	Line(const vec3& A, const vec3& B);

	vec3 GetOrigin() const;
	vec3 GetDestination() const;

protected:
	void InnerRender() const;
public:
	vec3 origin;
	vec3 destination;
};

// ============================================
class Plane : public Primitive
{
public:
	Plane(const vec3& normal = vec3(0,1,0));

	vec3 GetNormal() const;
protected:
	void InnerRender() const;
private:
	vec3 normal;
};

// ============================================
class PrimitiveDrawExamples : public Primitive
{
public:
	PrimitiveDrawExamples();
	PrimitiveDrawExamples(float size, vec3 origin);

	void DrawAllExamples();								// Will draw all the examples. All examples are drawn in Direct Mode.

public:
	void GL_PointsExample			(int index = 0);
	void GL_LinesExample			(int index = 1);
	void GL_LineStripExample		(int index = 2);
	void GL_LineLoopExample			(int index = 3);
	void GL_PolygonExample			(int index = 4);
	void GL_TrianglesExample		(int index = 5);
	void GL_TriangleStripExample	(int index = 6);
	void GL_TriangleFanExample		(int index = 7);
	void GL_QuadsExample			(int index = 8);
	void GL_QuadStripExample		(int index = 9);

public:
	vec3 origin;
	float size;											//Just used to scale everything uniformly.
};

#endif // !__PRIMITIVE_H__
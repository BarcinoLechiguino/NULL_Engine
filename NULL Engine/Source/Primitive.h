#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "glmath.h"
#include "Color.h"

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder
};

class Primitive
{
public:
	Primitive();

	void Update();
	virtual void	Render() const;
	void			SetPos(float x, float y, float z);
	vec3			GetPos() const;
	void			SetRotation(float angle, const vec3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

	Color color;
	mat4x4 transform;
	bool axis,wire;

protected:
	virtual void InnerRender() const;
	PrimitiveTypes type;
};

// ============================================
class Cube : public Primitive
{
public :
	Cube(const vec3& size = vec3(1.f,1.f,1.f), float mass = 1.f);

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

	bool loaded_in_array;
	bool loaded_in_indices;
};

// ============================================
class Sphere : public Primitive
{
public:
	Sphere(float radius = 1.f, float mass = 1.f);

	float GetRadius() const;
protected:
	void InnerRender() const;
private:
	float radius;
};

// ============================================
class Cylinder : public Primitive
{
public:
	Cylinder(float radius = 1.f, float height = 2.f, float mass = 1.f);

	float GetRadius() const;
	float GetHeight() const;
protected:
	void InnerRender() const;
private:
	float radius;
	float height;
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

	void DrawAllExamples();								// All examples are drawn in Direct Mode.

public:
	vec3 origin;
	float size;											//Just used to scale everything uniformly.
};

#endif // !__PRIMITIVE_H__
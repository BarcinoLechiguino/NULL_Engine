#include "MathGeoLib/src/Math/TransformOps.h"

#include "GameObject.h"

#include "C_Transform.h"

C_Transform::C_Transform(GameObject* owner) : Component(owner, COMPONENT_TYPE::TRANSFORM, "Transform"),
recalculate_local_transform(false)
{
	float4x4 rotation_mat;
	
	owner->matrix.Decompose(position, rotation_mat, scale);

	rotation = rotation_mat.ToEulerXYZ();

	int a = 0;

	if (owner->parent != nullptr)
	{
		//owner->parent->transform->position
	}
}

C_Transform::~C_Transform()
{

}

bool C_Transform::Update()
{
	bool ret = true;

	if (needs_translation)
	{
		owner->matrix.Translate(position);
		needs_translation = false;
	}

	if (needs_rotation)
	{
		owner->matrix.RotateX(rotation.x);
		owner->matrix.RotateY(rotation.y);
		owner->matrix.RotateZ(rotation.z);
		needs_rotation = false;
	}

	if (needs_scaling)
	{
		owner->matrix.Scale(scale);
		needs_scaling = false;
	}

	return ret;
}

bool C_Transform::CleanUp()
{
	bool ret = true;

	return ret;
}

// --- C_TRANSFORM METHODS ---
float3 C_Transform::GetPosition() const
{
	return position;
}

float3 C_Transform::GetRotation() const
{
	return rotation;
}

float3 C_Transform::GetScale() const
{
	return scale;
}

void C_Transform::SetPosition(const float3& position)
{
	this->position = position;

	needs_translation = true;
}

void C_Transform::SetRotation(const float3& rotation)
{
	this->rotation = rotation;

	needs_rotation = true;
}

void C_Transform::SetScale(const float3& scale)
{
	this->scale = scale; 

	needs_scaling = true;
}
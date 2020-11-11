#include "MathGeoLib/include/Math/TransformOps.h"

#include "GameObject.h"

#include "C_Transform.h"

C_Transform::C_Transform(GameObject* owner, float4x4 matrix) : Component(owner, COMPONENT_TYPE::TRANSFORM, "Transform"),
local_transform(matrix),
world_transform(matrix),
position(float3::zero),
rotation(float3::zero),
scale(float3::one),
update_local_transform(false),
update_world_transform(false)
{	
	if (owner == nullptr)
	{
		CleanUp();
	}
}

C_Transform::~C_Transform()
{

}

bool C_Transform::Update()
{
	bool ret = true;

	//Skip updating static elements. Truly static elements will not vary in any way, neither position, rotation or scale.
	if (update_local_transform)
	{
		UpdateLocalTransform();
	}

	if (update_world_transform)
	{
		UpdateWorldTransform();
	}

	return ret;
}

bool C_Transform::CleanUp()
{
	bool ret = true;

	return ret;
}

// --- C_TRANSFORM METHODS ---
void C_Transform::UpdateLocalTransform()
{
	// Update Local Transform: Add the new Position, Rotation and Scale to the matrix.
	//local_transform.SetTranslatePart(position);

	for (uint i = 0; i < owner->childs.size(); ++i)
	{
		owner->childs[i]->GetTransformComponent()->update_world_transform = true;
	}
	
	update_world_transform = true;
	update_local_transform = false;
}

void C_Transform::UpdateWorldTransform()
{
	if (owner->parent != nullptr)
	{
		world_transform = owner->parent->GetTransformComponent()->local_transform * local_transform;	// Transform should never be nullptr under any circumstances.
	}
	else
	{
		world_transform = local_transform;																// If the owner has no parent, then the world transform is the same as the local one.
	}

	update_world_transform = false;
}

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

	update_local_transform = true;
}

void C_Transform::SetRotation(const float3& rotation)
{
	this->rotation = rotation;

	update_local_transform = true;
}

void C_Transform::SetScale(const float3& scale)
{
	this->scale = scale; 

	update_local_transform = true;
}
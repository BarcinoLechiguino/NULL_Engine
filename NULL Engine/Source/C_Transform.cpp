#include "MathGeoLib/include/Math/TransformOps.h"
#include "MathGeoLib/include/Math/Quat.h"

#include "GameObject.h"

#include "C_Transform.h"

C_Transform::C_Transform(GameObject* owner, float4x4 matrix) : Component(owner, COMPONENT_TYPE::TRANSFORM, "Transform"),
local_transform(matrix),
world_transform(matrix),
position(float3::zero),
rotation(float3::zero),
scale(float3::one),
update_local_transform(true),
update_world_transform(false)
{	
	if (owner == nullptr)															// Safety check in case an ownerless transform is created.
	{
		CleanUp();
	}
	else
	{
		/*if (owner->parent != nullptr)
		{
			C_Transform* parent = owner->parent->GetTransformComponent();
			
			local_position = position - parent->position;
			local_rotation = rotation - parent->rotation;
			local_scale = scale - parent->scale;
		}
		else
		{
			local_position = position;
			local_rotation = rotation;
			local_scale = scale;
		}*/
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
void C_Transform::UpdateLocalTransform()																// Update Local Transform: Add the new Position, Rotation and Scale to the matrix.
{	
	//if (!update_world_transform)
	//{
	//	// Preparing the operations.
	//	Quat new_rotation = Quat::FromEulerXYZ(local_rotation.x, local_rotation.y, local_rotation.z);	// Generating a quaternion from the local_rotation vector. (From Euler to Quat)
	//	
	//	float3 curr_scale = local_transform.GetScale();													// Setting the local_transform's current scale to 0 to prevent the
	//	local_transform.Scale(curr_scale.Neg());														// new local_scale's value from being added to the previous.

	//	// Calculating the new local transform.
	//	local_transform.Scale(local_scale);																// Calculating the new transform with the new position, rotation and scale values.
	//	local_transform.SetRotatePart(new_rotation);													//
	//	local_transform.SetTranslatePart(local_position);												// -------------------------------------------------------------------------------
	//}
	//else
	//{
	//	if (owner->parent != nullptr)
	//	{
	//		C_Transform* parent = owner->parent->GetTransformComponent();

	//		local_position = position - parent->position;
	//		local_rotation = rotation - parent->rotation;
	//		local_scale = scale;
	//	}

	//	Quat new_rotation = Quat::FromEulerXYZ(local_rotation.x, local_rotation.y, local_rotation.z);	// Generating a quaternion from the local_rotation vector. (From Euler to Quat)

	//	float3 curr_scale = local_transform.GetScale();													// Setting the local_transform's current scale to 0 to prevent the
	//	local_transform.Scale(curr_scale.Neg());														// new local_scale's value from being added to the previous.

	//	local_transform.Scale(local_scale);																// Calculating the new transform with the new position, rotation and scale values.
	//	local_transform.SetRotatePart(new_rotation);													//
	//	local_transform.SetTranslatePart(local_position);												// -------------------------------------------------------------------------------
	//}

	if (owner->parent != nullptr)
	{
		float4x4 new_local = owner->parent->GetTransformComponent()->world_transform.Inverted() * local_transform;
		local_transform = new_local;
	}

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

	//position	= world_transform.TranslatePart();
	//rotation	= world_transform.ToEulerXYZ();
	//scale		= world_transform.GetScale();

	update_world_transform = false;
}

float4x4 C_Transform::GetLocalMatrix() const
{
	return local_transform;
}

float4x4 C_Transform::GetWorldMatrix() const
{
	return world_transform;
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

	update_world_transform = true;
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

//void C_Transform::Translate(const float3& movement)
//{
//	local_position += movement; 
//
//	update_local_transform = true;
//}
//
//void C_Transform::Rotate(const float3& rotation)
//{
//	local_rotation += rotation;
//
//	update_local_transform = true;
//}
//
//void C_Transform::Scale(const float3& scale)
//{
//	local_scale += scale;
//
//	update_local_transform = true;
//}
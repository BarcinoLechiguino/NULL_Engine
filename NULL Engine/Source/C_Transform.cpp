#include "MathGeoLib/include/Math/TransformOps.h"
#include "MathGeoLib/include/Math/Quat.h"

#include "GameObject.h"

#include "C_Transform.h"

C_Transform::C_Transform(GameObject* owner, float4x4 matrix) : Component(owner, COMPONENT_TYPE::TRANSFORM, "Transform"),
local_transform(matrix),
world_transform(matrix),
local_position(float3::zero),
local_rotation(float3::zero),
local_scale(float3::one),
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

	if (update_local_transform)										//Skip updating static elements. Truly static elements will not vary in any way, neither position, rotation or scale.
	{
		UpdateLocalTransform();
	}

	if (update_world_transform)
	{
		UpdateWorldTransform();
	}

	if (update_childs)
	{
		for (uint i = 0; i < owner->childs.size(); ++i)
		{
			//C_Transform* child = owner->childs[i]->GetTransformComponent();							// At the first problem with using a child that has yet to be transformed, change.
			//child->world_transform = world_transform * child->local_transform;

			owner->childs[i]->GetTransformComponent()->update_world_transform = true;
		}

		update_childs = false;
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
	if (owner->parent != nullptr)
	{
		local_transform = owner->parent->GetTransformComponent()->world_transform.Inverted() * world_transform;
	}
	else
	{
		local_transform = world_transform;
	}

	local_position			= local_transform.TranslatePart();
	local_rotation			= local_transform.ToEulerXYZ();
	local_scale				= local_transform.GetScale();
	
	update_local_transform	= false;
	update_world_transform	= true;
	update_childs			= true;
}

void C_Transform::UpdateWorldTransform()
{	
	if (owner->parent != nullptr)
	{
		world_transform = owner->parent->GetTransformComponent()->world_transform * local_transform;	// Transform should never be nullptr under any circumstances.
	}
	else
	{
		world_transform = local_transform;																// If the owner has no parent, then the world transform is the same as the local one.
	}

	update_world_transform = false;
	update_childs = true;
}

float4x4 C_Transform::GetLocalTransform() const
{
	return local_transform;
}

float4x4 C_Transform::GetWorldTransform() const
{
	return world_transform;
}

void C_Transform::SetLocalTransform(const float4x4& local_transform)
{	
	this->local_transform = local_transform;

	local_position	= this->local_transform.TranslatePart();
	local_rotation	= this->local_transform.ToEulerXYZ();
	local_scale		= this->local_transform.GetScale();

	// Should be local_scale = world_transform.GetScale() * parent->world_transform.GetScale();

	update_local_transform = true;
}

void C_Transform::SetWorldTransform(const float4x4& world_transform)
{
	this->world_transform = world_transform;

	update_world_transform = true;
}

float3 C_Transform::GetWorldPosition() const
{
	return world_transform.TranslatePart();
}

float3 C_Transform::GetWorldRotation() const
{
	return world_transform.ToEulerXYZ();
}

float3 C_Transform::GetWorldScale() const
{
	return world_transform.GetScale();
}

void C_Transform::SetPosition(const float3& position, const bool& is_local)
{
	if (is_local)
	{
 		local_transform.SetTranslatePart(position);
		local_position = local_transform.TranslatePart();

		update_world_transform = true;
	}
	else
	{
		world_transform.SetTranslatePart(position);

		update_local_transform = true;
	}
}

void C_Transform::SetRotation(const float3& rotation, const bool& is_local)
{
	if (is_local)
	{
		local_transform.SetRotatePart(Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z));
		local_rotation = local_transform.ToEulerXYZ();

		update_world_transform = true;
	}
	else
	{
		world_transform.SetRotatePart(Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z));
		
		update_local_transform = true;
	}
}

void C_Transform::SetRotation(const Quat& rotation, const bool& is_local)
{
	if (is_local)
	{
		local_transform = local_transform * rotation;
		local_rotation = rotation.ToEulerXYZ();

		update_world_transform = true;
	}
	else
	{
		world_transform = world_transform * rotation;

		update_local_transform = true;
	}
}

void C_Transform::SetScale(const float3& scale, const bool& is_local)
{
	if (is_local)
	{
		local_transform.Scale(local_transform.GetScale().Neg());
		local_transform.Scale(scale);
		local_scale = local_transform.GetScale();

		update_world_transform = true;
	}
	else
	{
		world_transform.Scale(world_transform.GetScale().Neg());
		world_transform.Scale(scale);
		
		update_local_transform = true;
	}
}

void C_Transform::Translate(const float3& movement, const bool& is_local)
{
	if (is_local)
	{
		local_transform = local_transform * float4x4::Translate(movement);
		local_position = local_transform.TranslatePart();

		update_world_transform = true;
	}
	else
	{
		world_transform = world_transform * float4x4::Translate(movement);

		update_local_transform = true;
	}
}

void C_Transform::Rotate(const float3& rotation, const bool& is_local)
{
	if (is_local)
	{
		Quat X_rotation = Quat::RotateAxisAngle(local_transform.WorldX(), rotation.x);
		Quat Y_rotation = Quat::RotateAxisAngle(local_transform.WorldY(), rotation.y);
		Quat Z_rotation = Quat::RotateAxisAngle(local_transform.WorldZ(), rotation.z);
		
		local_transform = local_transform * X_rotation * Y_rotation * Z_rotation;
		local_rotation = local_transform.ToEulerXYZ();

		update_world_transform = true;
	}
	else
	{
		
		
		update_local_transform = true;
	}
}

void C_Transform::Scale(const float3& scale, const bool& is_local)
{
	if (is_local)
	{
		local_scale += scale;

		update_world_transform = true;
	}
	else
	{


		update_local_transform = true;
	}
}
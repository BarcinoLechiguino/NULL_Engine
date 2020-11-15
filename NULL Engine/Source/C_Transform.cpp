#include "Application.h"
#include "E_Scene.h"

#include "GameObject.h"

#include "C_Transform.h"

C_Transform::C_Transform(GameObject* owner, float4x4 matrix) : Component(owner, COMPONENT_TYPE::TRANSFORM, "Transform"),
local_transform(matrix),
world_transform(matrix),
update_local_transform(false),
update_world_transform(false)
{	
	local_transform.Decompose(local_position, local_rotation, local_scale);
}

C_Transform::~C_Transform()
{

}

bool C_Transform::Update()
{
	bool ret = true;

	//if (update_local_transform)
	//{
	//	UpdateLocalTransform();
	//}

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

// ------ C_TRANSFORM METHODS ------
// --- TRANSFORMS/MATRICES
void C_Transform::UpdateLocalTransform()
{
	local_transform = float4x4::FromTRS(local_position, local_rotation, local_scale);

	update_local_transform = false;
	update_world_transform = true;
}

void C_Transform::UpdateWorldTransform()
{
	if (owner->parent != nullptr)
	{
		world_transform = owner->parent->GetTransformComponent()->world_transform * local_transform;
	}
	else
	{
		world_transform = local_transform;
	}

	for (uint i = 0; i < owner->childs.size(); ++i)
	{
		owner->childs[i]->GetTransformComponent()->update_world_transform = true;
	}

	update_world_transform = false;
}

void C_Transform::SyncWorldToLocal()
{
	if (owner->parent != nullptr)
	{
		world_transform = owner->parent->GetTransformComponent()->world_transform * local_transform;
	}
	else
	{
		world_transform = local_transform;
	}

	for (uint i = 0; i < owner->childs.size(); ++i)
	{
		owner->childs[i]->GetTransformComponent()->update_world_transform = true;
	}
}

void C_Transform::SyncLocalToWorld()
{
	if (owner->parent != nullptr)
	{
		local_transform = owner->parent->GetTransformComponent()->world_transform.Inverted() * world_transform;
	}
	else
	{
		local_transform = world_transform;
	}

	local_position	= local_transform.TranslatePart();
	local_rotation	= local_transform.RotatePart().ToQuat();
	local_scale		= local_transform.GetScale();

	for (uint i = 0; i < owner->childs.size(); ++i)
	{
		owner->childs[i]->GetTransformComponent()->update_world_transform = true;
	}
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

	local_position	= local_transform.TranslatePart();
	float3 euler	= local_transform.RotatePart().ToEulerXYZ();
	local_rotation	= Quat::FromEulerXYZ(euler.x, euler.y, euler.z);
	local_scale		= local_transform.GetScale();

	update_world_transform = true;
}

void C_Transform::SetWorldTransform(const float4x4& world_transform)
{
	this->world_transform = world_transform;

	SyncLocalToWorld();
}

// --- POSITION, ROTATION AND SCALE METHODS
// -- GET METHODS
float3 C_Transform::GetLocalPosition() const
{
	return local_position;
}

Quat C_Transform::GetLocalRotation() const
{
	return local_rotation;
}

float3 C_Transform::GetLocalEulerRotation() const
{
	return local_rotation.ToEulerXYZ();
}

float3 C_Transform::GetLocalScale() const
{
	return local_scale;
}

float3 C_Transform::GetWorldPosition() const
{
	return world_transform.TranslatePart();
}

Quat C_Transform::GetWorldRotation() const
{
	return world_transform.RotatePart().ToQuat();
}

float3 C_Transform::GetWorldEulerRotation() const
{
	return world_transform.RotatePart().ToEulerXYZ();
}

float3 C_Transform::GetWorldScale() const
{
	return world_transform.GetScale();
}

// -- SET METHODS
void C_Transform::SetLocalPosition(const float3& new_position)
{
	local_position = new_position;

	UpdateLocalTransform();
	//update_local_transform = true;															// Parameter modifications could be batched to re-calculate the local transform only once.
}																								// However, this would allow access to the dirty local transform before it can be updated.

void C_Transform::SetLocalRotation(const Quat& new_rotation)
{
	local_rotation = new_rotation;

	UpdateLocalTransform();
}

void C_Transform::SetLocalRotation(const float3& new_rotation)
{
	local_rotation = Quat::FromEulerXYZ(new_rotation.x, new_rotation.y, new_rotation.z);

	UpdateLocalTransform();
}

void C_Transform::SetLocalScale(const float3& new_scale)
{
	local_scale = new_scale;

	UpdateLocalTransform();
}

void C_Transform::SetWorldPosition(const float3& new_position)
{
	world_transform.SetTranslatePart(new_position);

	SyncLocalToWorld();
}

void C_Transform::SetWorldRotation(const Quat& new_rotation)
{
	world_transform.SetRotatePart(new_rotation);

	SyncLocalToWorld();
}

void C_Transform::SetWorldRotation(const float3& new_rotation)
{
	world_transform.SetRotatePart(Quat::FromEulerXYZ(new_rotation.x, new_rotation.y, new_rotation.z));

	SyncLocalToWorld();
}

void C_Transform::SetWorldScale(const float3& new_scale)
{
	world_transform.Scale(world_transform.GetScale().Neg());
	world_transform.Scale(new_scale);

	SyncLocalToWorld();
}

// --- ADDING/SUBUTRACTING FROM POSITION, ROTATION AND SCALE
void C_Transform::Translate(const float3& velocity)
{
	local_position += velocity;

	UpdateLocalTransform();
}

void C_Transform::Rotate(const Quat& angular_velocity)
{
	local_rotation = local_rotation * angular_velocity;

	UpdateLocalTransform();
}

void C_Transform::Rotate(const float3& angular_velocity)
{
	local_rotation = local_rotation * Quat::FromEulerXYZ(angular_velocity.x, angular_velocity.y, angular_velocity.z);

	UpdateLocalTransform();
}

void C_Transform::Scale(const float3& expansion_rate)
{
	local_scale += expansion_rate;

	UpdateLocalTransform();
}

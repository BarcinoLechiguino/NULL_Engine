#ifndef __C_TRANSFORM_H__
#define __C_TRANSFORM_H__

#include "MathStructures.h"

#include "Component.h"

class GameObject;

//struct Transform											// Maybe use this struct instead of having 3 separate variables? Would be useful to optimize glPushMatrix()/glPopMatrix() calls.
//{
//	float3 postion;
//	float3 rotation;
//	float3 scale;
//};

class C_Transform : public Component
{
public:
	C_Transform(GameObject* owner, float4x4 matrix = float4x4::identity);
	~C_Transform();

	bool Update() override;
	bool CleanUp() override;

public:																// --------------------------------------------------------- TRANSFORM METHODS
	void		UpdateLocalTransform();								// Updates the local_transform matrix.
	void		UpdateWorldTransform();								// Updates the world_transform matrix.

	float4x4	GetLocalTransform() const;							// Returns the local transform.
	float4x4	GetWorldTransform() const;							// Returns the world transform.
	void		SetLocalTransform(const float4x4& local_transform);	// Sets the local transform with the given matrix.
	void		SetWorldTransform(const float4x4& world_transform);	// Sets the world transform with the given matrix.

public:																// --------------------------------------------------------- POSITION, ROTATION & SCALE METHODS
	float3		GetLocalPosition() const;
	float3		GetLocalRotation() const;
	float3		GetLocalScale() const;
	
	float3		GetWorldPosition() const;							// Returns the global Transform's position.
	float3		GetWorldRotation() const;							// Returns the global Transform's rotation in Radiants.
	float3		GetWorldScale() const;								// Returns the global Transform's scale.

	void		SetPosition(const float3& new_position, const bool& is_local = true);	// Mod. world_trns's position. Ex: SetPosition(1.0f, 0.0f, 0.0f)  ==> wld_position = (1.0f, 0.0f, 0.0f);
	void		SetRotation(const float3& new_rotation, const bool& is_local = true);	// Mod. world_trns's rotation. Ex: SetRotation(90.0f, 0.0f, 0.0f) ==> wld_rotation = (90.0f,0.0f, 0.0f);
	void		SetRotation(const Quat& new_rotation, const bool& is_local = true);		// Mod. world_trns's rotation. Same as above but using Quats.
	void		SetScale(const float3& new_scale, const bool& is_local = true);			// Mod. world_trns's scale.	   Ex: SetScale(1.0f, 1.0f, 1.0f)	  ==> wld_scale	= (1.0f, 1.0f, 1.0f);

	void		Translate(const float3& movement, const bool& is_local = true);		// Add/Sub to local_trns's position. Ex: Translate(1.0f, 0.0f, 0.0f)  ==> lcl_position + (1.0f, 0.0f, 0.0f);
	void		Rotate(const float3& rotation, const bool& is_local = true);		// Add/Sub to local_trns's rotation. Ex: Rotate(90.0f, 0.0f, 0.0f)	  ==> lcl_rotation + (90.0f,0.0f, 0.0f);
	void		Rotate(const Quat& rotation, const bool& is_local = true);			// Add/Sub to local_trns's rotation. Same as above but using Quats.
	void		Scale(const float3& scale, const bool& is_local = true);			// Add/Sub to local_trns's scale.	 Ex: Scale(1.0f, 0.0f, 0.0f)	  ==> lcl_scale	+ (1.0f, 0.0f, 0.0f);

public:
	void		ImportTransform(const float3& position, const Quat& rotation, const float3& scale);

public:
	bool		update_local_transform;								// Dirty Flag that will be Set (true) if the position, rotation or/and scale of the local transform have been modified.
	bool		update_world_transform;								// Dirty Flag that will be Set (true) if the local transform of the parent of this component's owner has been modified.
	bool		update_childs;

private:
	float4x4	local_transform;									// Matrix that will keep track of the position, rotation and scale of the owner in reference to it's parent
	float4x4	world_transform;									// Matrix that will keep track of the position, rotation and scale of the owner in reference to the World.
	
	float3		local_position;										// Position in reference to the parent. world_position will be calculated through world_transform.TranslatePart();
	float3		local_rotation;										// Rotation in reference to the parent.	world_rotation will be calculated through world_transform.ToEulerXYZ();
	float3		local_scale;										// Scale in reference to the parent.	world_scale will be calculated through world_transform.GetScale();
	
	//Transform	transform;
};

#endif // !_C_TRANSFORM_H__
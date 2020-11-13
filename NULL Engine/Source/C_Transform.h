#ifndef __C_TRANSFORM_H__
#define __C_TRANSFORM_H__

#include "Component.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/float3.h"

class GameObject;

struct Transform											// Maybe use this struct instead of having 3 separate variables? Would be useful to optimize glPushMatrix()/glPopMatrix() calls.
{
	float3 postion;
	float3 rotation;
	float3 scale;
};

class C_Transform : public Component
{
public:
	C_Transform(GameObject* owner, float4x4 matrix = float4x4::identity);
	~C_Transform();

	bool Update() override;
	bool CleanUp() override;

public:																// --------------------------------------------------------- TRANSFORM METHODS
	void		UpdateLocalTransform();								// Update, change, refresh, recalculate
	void		UpdateWorldTransform();								// Using glLoadIdentiy() along with glTranslate() glRotate() glScale() is faster than using glLoadMatrix and glMultMatrix().

	float4x4	GetLocalTransform() const;							// Returns the local transform.
	float4x4	GetWorldTransform() const;							// Returns the world transform.
	void		SetLocalTransform(const float4x4& local_transform);	// Sets the local transform with the given matrix.
	void		SetWorldTransform(const float4x4& world_transform);	// Sets the world transform with the given matrix.

public:																// --------------------------------------------------------- POSITION, ROTATION & SCALE METHODS
	float3		GetWorldPosition() const;							// Returns the global Transform's position.
	float3		GetWorldRotation() const;							// Returns the global Transform's rotation in Radiants.
	float3		GetWorldScale() const;								// Returns the global Transform's scale.

	void		SetPosition(const float3& position, const bool& is_local = true);	// Mod. world_trns's position.	 Ex: SetPosition(1.0f, 0.0f, 0.0f)	==> world_position = (1.0f, 0.0f, 0.0f);
	void		SetRotation(const float3& rotation, const bool& is_local = true);	// Mod. world_trns's rotation.	 Ex: SetRotation(90.0f, 0.0f, 0.0f)	==> world_rotation = (90.0f,0.0f, 0.0f);
	void		SetScale(const float3& scale, const bool& is_local = true);			// Mod. world_trns's scale.		 Ex: SetScale(1.0f, 1.0f, 1.0f)		==> world_scale	= (1.0f, 1.0f, 1.0f);

	void		Translate(const float3& movement, const bool& is_local = true);		// +/- to local_trns's position. Ex: Translate(1.0f, 0.0f, 0.0f)	==> local_position + (1.0f, 0.0f, 0.0f);
	void		Rotate(const float3& rotation, const bool& is_local = true);		// +/- to local_trns's rotation. Ex: Rotate(90.0f, 0.0f, 0.0f)		==> local_rotation + (90.0f,0.0f, 0.0f);
	void		Scale(const float3& scale, const bool& is_local = true);			// +/- to local_trns's scale.	 Ex: Scale(1.0f, 0.0f, 0.0f)		==> local_scale	+ (1.0f, 0.0f, 0.0f);

	void		SetRotation(const Quat& rotation);					// Same as their float3 counterparts but passing a Quat. as the arg. Related with importing Transformations with Assimp.
	void		Rotate(const Quat& rotation);						// Same as their float3 counterparts but passing a Quat. as the arg. Related with importing Transformations with Assimp.

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
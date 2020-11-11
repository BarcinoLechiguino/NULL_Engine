#ifndef __C_TRANSFORM_H__
#define __C_TRANSFORM_H__

#include "Component.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/float3.h"

class GameObject;

struct Transform									// Maybe use this struct instead of having 3 separate variables? Would be useful to optimize glPushMatrix()/glPopMatrix() calls.
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

public:
	void UpdateLocalTransform();					// Update, change, refresh, recalculate
	void UpdateWorldTransform();					// Using glLoadIdentiy() along with glTranslate() glRotate() glScale() is faster than using glLoadMatrix and glMultMatrix().
	
	float4x4 GetLocalMatrix() const;
	float4x4 GetGlobalMatrix() const;

	float3 GetPosition() const;
	float3 GetRotation() const;
	float3 GetScale() const;
	
	void SetPosition(const float3& position);
	void SetRotation(const float3& rotation);
	void SetScale(const float3& scale);

public:
	float4x4	local_transform;					// Matrix that will keep track of the position, rotation and scale of the owner in reference to it's parent
	float4x4	world_transform;
	
	bool		update_local_transform;				// Dirty Flag that will be Set (true) if the position, rotation or/and scale of the local transform have been modified.
	bool		update_world_transform;				// Dirty Flag that will be Set (true) if the local transform of the parent of this component's owner has been modified.

private:
	float3		position;
	float3		rotation;
	float3		scale;

	//Transform	transform;
};

#endif // !_C_TRANSFORM_H__
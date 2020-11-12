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

public:
	void UpdateLocalTransform();							// Update, change, refresh, recalculate
	void UpdateWorldTransform();							// Using glLoadIdentiy() along with glTranslate() glRotate() glScale() is faster than using glLoadMatrix and glMultMatrix().
	
	float4x4 GetLocalMatrix() const;
	float4x4 GetWorldMatrix() const;

	float3 GetPosition() const;								// Returns the global Transform's position.
	float3 GetRotation() const;								// Returns the global Transform's rotation.
	float3 GetScale() const;								// Returns the global Transform's scale.

	void SetPosition(const float3& position);				// Modifies the world Transform's position.		Ex: SetPosition(1.0f, 0.0f, 0.0f)	==> world_position	= (1.0f, 0.0f, 0.0f);
	void SetRotation(const float3& rotation);				// Modifies the world Transform's rotation.		Ex: SetRotation(90.0f, 0.0f, 0.0f)	==> world_rotation	= (90.0f, 0.0f, 0.0f);
	void SetScale(const float3& scale);						// Modifies the world Transform's scale.		Ex: SetScale(1.0f, 1.0f, 1.0f)		==> world_scale		= (1.0f, 1.0f, 1.0f); 

	void Translate(const float3& movement);					// Modifies the local Transform's position.		Ex: Translate(1.0f, 0.0f, 0.0f)		==> local_position	+ (1.0f, 0.0f, 0.0f);
	void Rotate(const float3& rotation);					// Modifies the local Transform's rotation.		Ex: Rotate(90.0f, 0.0f, 0.0f)		==> local_rotation	+ (90.0f, 0.0f, 0.0f);
	void Scale(const float3& scale);						// Modifies the local Transform's scale.		Ex: Scale(1.0f, 0.0f, 0.0f)			==> local_scale		+ (1.0f, 0.0f, 0.0f);

	void SetScaleToZero(float4x4 transform);

public:
	float4x4	local_transform;							// Matrix that will keep track of the position, rotation and scale of the owner in reference to it's parent
	float4x4	world_transform;
	
	bool		update_local_transform;						// Dirty Flag that will be Set (true) if the position, rotation or/and scale of the local transform have been modified.
	bool		update_world_transform;						// Dirty Flag that will be Set (true) if the local transform of the parent of this component's owner has been modified.

private:
	float3		position;									// Local position.	Will be used for display + transform modification through the editor.
	float3		rotation;									// Local rotation.	Will be used for display + transform modification through the editor.
	float3		scale;										// Local scale.		Will be used for display + transform modification through the editor.



	// float3		local_position;								// Local position.	Using dirty flags to update the local transform. (TBD)
	// float3		local_rotation;								// Local rotation.	Using dirty flags to update the local transform. (TBD)
	// float3		local_scale;								// Local scale.		Using dirty flags to update the local transform. (TBD)

	//Transform	transform;
};

#endif // !_C_TRANSFORM_H__
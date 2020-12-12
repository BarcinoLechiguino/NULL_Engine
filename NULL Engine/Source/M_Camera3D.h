#ifndef __CAMERA_3D_H__
#define __CAMERA_3D_H__

#include "MathGeoLib/include/Geometry/LineSegment.h";
#include "Module.h"

#include "glmath.h"

class ParsonNode;
class GameObject;
class C_Camera;

class M_Camera3D : public Module
{
public:
	M_Camera3D(bool is_active = true);
	~M_Camera3D();

	bool			Init				(ParsonNode& root) override;
	bool			Start				() override;
	UPDATE_STATUS	Update				(float dt) override;
	bool			CleanUp				() override;

	bool			LoadConfiguration	(ParsonNode& root) override;
	bool			SaveConfiguration	(ParsonNode& root) const override;

public:
	void			CreateMasterCamera				();
	C_Camera*		GetCurrentCamera				() const;
	void			SetCurrentCamera				(C_Camera* c_camera);
	void			SetMasterCameraAsCurrentCamera	();

public:
	void			PointAt					(const vec3& position, const vec3& reference, bool RotateAroundReference = false);
	void			LookAt					(const float3& Spot);
	void			Focus					(const vec3& target_position, const float& distance_from_target = 10.0f);
	void			Move					(const vec3& Movement);

	void			ReturnToWorldOrigin		();

public:																		// Camera3D Getters and Setters.
	float*			GetRawViewMatrix		();
	mat4x4			GetViewMatrix			();

	//vec3			GetPosition				() const;
	float3			GetPosition				() const;
	vec3			GetReference			() const;
	vec3			GetSpot					() const;
	void			SetPosition				(const vec3& position);
	void			SetReference			(const vec3& reference);
	void			SetSpot					(const vec3& spot);

	float			GetMovementSpeed		() const;
	float			GetRotationSpeed		() const;
	float			GetZoomSpeed			() const;
	void			SetMovementSpeed		(const float& movement_speed);
	void			SetRotationSpeed		(const float& rotation_speed);
	void			SetZoomSpeed			(const float& zoom_speed);

	float3			GetMasterCameraPosition	() const;
	float3			GetMasterCameraRotation	() const;
	float3			GetMasterCameraScale	() const;
	void			SetMasterCameraPosition	(const float3& position);
	void			SetMasterCameraRotation	(const float3& rotation);
	void			SetMasterCameraScale	(const float3& scale);

private:
	void			CalculateViewMatrix		();								// Calculates both the worldspace matrix and the viewspace matrix.

	void			WASDMovement			();								// Translates the camera along XYZ (Right, Up, Forward), which will act as the camera's axis.
	void			FreeLookAround			();								// Receives information about the mouse's motion values and rotates the camera on it's axis.
	void			RotateAroundReference	();								// Will rotate the camera around the point specified by the reference var.. Almost identical to FreeLookAround();
	void			PanCamera				();								// Will translate the camera along XY (Right, Up). Moving the camera in the plane of the camera frame.
	void			Zoom					();								// Will translate the camera along the Z (Forward) axis. The camera will move forwards and backwards. 

public:
	GameObject*		master_camera;
	C_Camera*		current_camera;
	LineSegment		ray;

	vec3 X;																	// Right vector of the Camera. Will always remain axis aligned in this implementation.
	vec3 Y;																	// Up vector of the Camera.
	vec3 Z;																	// Forward Vector of the Camera. Represents where the camera is pointing to.

	float3 ref;

	vec3 position_origin;													// 
	vec3 reference_origin;													//
	vec3 position;															// Position of the Camera in the World Space.
	vec3 reference;															// Point where the camera will look at and rotate around.

	float movement_speed;													// 
	float rotation_speed;													// 
	float zoom_speed;														// 

private:
	mat4x4 ViewMatrix;
	mat4x4 ViewMatrixInverse;
};

#endif // !__CAMERA_3D_H__
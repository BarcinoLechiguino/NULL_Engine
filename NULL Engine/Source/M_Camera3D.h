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
	void			PointAt							(const float3& position, const float3& reference, bool RotateAroundReference = false);
	void			LookAt							(const float3& Spot);
	void			Focus							(const float3& target_position, const float& distance_from_target = 10.0f);
	void			Move							(const float3& velocity);

	void			ReturnToWorldOrigin				();

public:																				// Camera3D Getters and Setters.
	float3			GetPosition						() const;
	float3			GetReference					() const;
	void			SetPosition						(const float3& position);
	void			SetReference					(const float3& reference);

	float			GetMovementSpeed				() const;
	float			GetRotationSpeed				() const;
	float			GetZoomSpeed					() const;
	void			SetMovementSpeed				(const float& movement_speed);
	void			SetRotationSpeed				(const float& rotation_speed);
	void			SetZoomSpeed					(const float& zoom_speed);

	float3			GetMasterCameraPosition			() const;
	float3			GetMasterCameraRotation			() const;
	float3			GetMasterCameraScale			() const;
	void			SetMasterCameraPosition			(const float3& position);
	void			SetMasterCameraRotation			(const float3& rotation);
	void			SetMasterCameraScale			(const float3& scale);

private:
	void			WASDMovement					();								// Translates the camera along XYZ (Right, Up, Forward), which will act as the camera's axis.
	void			FreeLookAround					();								// Receives information about the mouse's motion values and rotates the camera on it's axis.
	void			RotateAroundReference			();								// Will rotate the camera around a point specified by the reference var.. ~identical to FreeLookAround();
	void			PanCamera						();								// Will translate the camera along XY (Right, Up). Moving the camera in the plane of the camera frame.
	void			Zoom							();								// Will translate the camera along the Z (Forward) axis. The camera will move forwards and backwards. 

public:
	GameObject*		master_camera;
	C_Camera*		current_camera;
	LineSegment		ray;

	float3			reference;														// Point where the camera will look at and rotate around.

	float3			position_origin;												// 
	float3			reference_origin;												//

	float			movement_speed;													// 
	float			rotation_speed;													// 
	float			zoom_speed;														// 
};

#endif // !__CAMERA_3D_H__
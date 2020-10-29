#ifndef __CAMERA_3D_H__
#define __CAMERA_3D_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class Configuration;

class M_Camera3D : public Module
{
public:
	M_Camera3D(bool is_active = true);
	~M_Camera3D();

	bool			Init(Configuration& root) override;
	bool			Start() override;
	UPDATE_STATUS	Update(float dt) override;
	bool			CleanUp() override;

	bool			LoadConfiguration(Configuration& root) override;
	bool			SaveConfiguration(Configuration& root) const override;

public:
	void			Look(const vec3 &position, const vec3 &reference, bool RotateAroundReference = false);
	void			LookAt(const vec3 &Spot);
	void			Move(const vec3 &Movement);

public:																											// Camera3D Getters and Setters.
	float*			GetRawViewMatrix();
	mat4x4			GetViewMatrix();

	vec3			GetPosition() const;
	vec3			GetReference() const;
	vec3			GetSpot() const;
	void			SetPosition(vec3 position);
	void			SetReference(vec3 reference);
	void			SetSpot(vec3 spot);

	float			GetMovementSpeed() const;
	float			GetRotationSpeed() const;
	float			GetZoomSpeed() const;
	void			SetMovementSpeed(float movement_speed);
	void			SetRotationSpeed(float rotation_speed);
	void			SetZoomSpeed(float zoom_speed);


private:
	void			CalculateViewMatrix();

	void			WASDMovement();									// Translates the camera along XYZ (Right, Up, Forward), which will act as the camera's axis.
	void			FreeLookAround();								// Receives information about the mouse's motion values and rotates the camera on it's axis.
	void			RotateAroundReference();						// Will rotate the camera around the point specified by the reference variable. Almost identical to FreeLookAround();
	void			PanCamera();									// Will translate the camera along XY (Right, Up). The movement can be described as moving in the plane of the camera frame.
	void			Zoom();											// Will translate the camera along the Z (Forward) axis. This means that the camera will move forwards and backwards. 

	void			ReturnToWorldOrigin();

public:
	vec3 X;															// Right vector of the Camera. Will always remain axis aligned in this implementation.
	vec3 Y;															// Up vector of the Camera.
	vec3 Z;															// Forward Vector of the Camera. Represents where the camera is pointing to.

	vec3 position;													// Position of the Camera in the World Space.
	vec3 reference;													// Point where the camera will look at and rotate around.

	float movement_speed;											// 
	float rotation_speed;											// 
	float zoom_speed;												// 

private:
	mat4x4 ViewMatrix;
	mat4x4 ViewMatrixInverse;
};

#endif // !__CAMERA_3D_H__
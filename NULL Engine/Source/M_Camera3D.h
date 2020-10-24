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
	void			Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void			LookAt(const vec3 &Spot);
	void			Move(const vec3 &Movement);

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

public:
	vec3 X;
	vec3 Y;
	vec3 Z;

	vec3 Position;
	vec3 Reference;

	float movement_speed;
	float rotation_speed;
	float zoom_speed;

private:
	mat4x4 ViewMatrix;
	mat4x4 ViewMatrixInverse;
};

#endif // !__CAMERA_3D_H__
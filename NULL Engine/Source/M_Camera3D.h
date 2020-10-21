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

	bool Init(Configuration& root) override;
	bool Start() override;
	UPDATE_STATUS Update(float dt) override;
	bool CleanUp() override;

	bool SaveConfiguration(Configuration& root) const override;
	bool LoadConfiguration(Configuration& root) override;

public:
	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetRawViewMatrix();
	mat4x4 GetViewMatrix();

private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
};

#endif // !__CAMERA_3D_H__
#ifndef __C_CAMERA_H__
#define __C_CAMERA_H__

#include "MathGeoFrustum.h"
#include "Component.h"

class ParsonNode;
class GameObject;

typedef unsigned int uint;

class C_Camera : public Component
{
public:
	C_Camera(GameObject* owner);
	~C_Camera();

	bool Update		() override;
	bool CleanUp	() override;

	bool SaveState	(ParsonNode& configuration) const override;
	bool LoadState	(ParsonNode& configuration) override;

public:


private:
	Frustum frustum;

	uint horizontal_fov;
	uint vertical_fov;

	float near_plane_size;
	float far_plane_size;

	bool orthogonal_view;
	bool hide_frustum;
};

#endif // !__CAMERA_H__
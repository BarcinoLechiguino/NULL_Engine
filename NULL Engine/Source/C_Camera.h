#ifndef __C_CAMERA_H__
#define __C_CAMERA_H__

#include "MathGeoFrustum.h"
#include "MathGeoLib/include/Math/float2.h"
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

	bool SaveState	(ParsonNode& root) const override;
	bool LoadState	(ParsonNode& root) override;

public:																										// --- FRUSTUM METHODS
	void		InitFrustum();
	void		UpdateFrustum				();

	void		UpdateFrustumPlanes			();
	void		UpdateFrustumVertices		();
	Plane*		GetFrustumPlanes			() const;
	float3*		GetFrustumVertices			() const;

	float		ComputeAspectRatio			(const uint& horizontal_fov, const uint& vertical_fov);
	float3x4	GetViewMatrix				();
	float4x4	GetProjectionMatrix			();

	bool		FrustumCointainsAABB		(const AABB& aabb) const;										// Returns true if the Frustum contains the whole AABB.
	bool		FrustumIntersectsAABB		(const AABB& aabb) const;										// Returns true if the Frustum contains at least one vertex of the AABB.

public:																										// --- GET/SET FRUSTUM CHARACTERISTICS
	float		GetNearPlaneDistance	() const;
	float		GetFarPlaneDistance		() const;
	float2		GetNearPlaneSize		() const;
	float2		GetFarPlaneSize			() const;
	uint		GetHorizontalFOV		() const;
	uint		GetVerticalFOV			() const;

	void		SetNearPlaneDistance	(const float& near_distance);
	void		SetFarPlaneDistance		(const float& far_distance);
	void		SetNearPlaneSize		(const float2& near_plane_size);
	void		SetFarPlaneSize			(const float2& far_plane_size);
	void		SetHorizontalFOV		(const uint& horizontal_fov);
	void		SetVerticalFOV			(const uint& vertical_fov);

	void		GetMinMaxFOV			(uint& min_fov, uint& max_fov);
	void		SetMinMaxFOV			(const uint& min_fov, const uint& max_fov);

public:																										// --- CAMERA FLAGS	
	bool		IsCulling				() const;
	bool		OrthogonalView			() const;
	bool		FrustumIsHidden			() const;

	void		SetIsCulling			(const bool& set_to);
	void		SetOrthogonalView		(const bool& set_to);
	void		SetFrustumIsHidden		(const bool& set_to);

	bool		IsSceneCamera			() const;
	void		SetIsSceneCamera		(const bool& set_to);

private:
	Frustum frustum;
	Plane*	frustum_planes;
	float3* frustum_vertices;

	uint	horizontal_fov;
	uint	vertical_fov;

	uint	min_fov;
	uint	max_fov;

	float2	near_plane_size;
	float2	far_plane_size;

	bool	culling;
	bool	orthogonal_view;
	bool	hide_frustum;

	bool	is_scene_camera;
};

#endif // !__CAMERA_H__
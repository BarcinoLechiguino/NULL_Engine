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
	void		InitFrustum					();
	void		UpdateFrustumTransform		();
	Frustum		GetFrustum					() const;

	void		SetUpdateFrustumTransform	(const bool& set_to);
	void		SetUpdateProjectionMatrix	(const bool& set_to);
	bool		GetUpdateFrustumTransform	() const;
	bool		GetUpdateProjectionMatrix	() const;

	float*		GetOGLViewMatrix			();
	float*		GetOGLProjectionMatrix		();

public:																										// --- CAMERA TRANSFORM
	void PointAt();
	void LookAt();
	void Move(const float3& movement);

	void SetPosition();

public:																										// --- FRUSTUM CULLING
	void		UpdateFrustumPlanes			();
	void		UpdateFrustumVertices		();
	Plane*		GetFrustumPlanes			() const;
	float3*		GetFrustumVertices			() const;

	bool		FrustumCointainsAABB		(const AABB& aabb) const;										// Returns true if the Frustum contains the whole AABB.
	bool		FrustumIntersectsAABB		(const AABB& aabb) const;										// Returns true if the Frustum contains at least one vertex of the AABB.

public:																										// --- GET/SET FRUSTUM SETTINGS
	float		GetAspectRatio				() const;
	void		SetAspectRatio				(const float& aspect_ratio);

	float		GetNearPlaneDistance		() const;
	float		GetFarPlaneDistance			() const;
	float		GetHorizontalFOV			() const;
	float		GetVerticalFOV				() const;

	void		SetNearPlaneDistance		(const float& near_distance);
	void		SetFarPlaneDistance			(const float& far_distance);
	void		SetHorizontalFOV			(const float& horizontal_fov);									// Chosen FOV adaptation: VFOV locked , HFOV adapted to aspect_ratio.
	void		SetVerticalFOV				(const float& vertical_fov);

	void		GetMinMaxFOV				(uint& min_fov, uint& max_fov) const;
	void		SetMinMaxFOV				(const uint& min_fov, const uint& max_fov);

public:																										// --- CAMERA FLAGS	
	bool		IsCulling					() const;
	bool		OrthogonalView				() const;
	bool		FrustumIsHidden				() const;

	void		SetIsCulling				(const bool& set_to);
	void		SetOrthogonalView			(const bool& set_to);
	void		SetFrustumIsHidden			(const bool& set_to);

	bool		IsSceneCamera				() const;
	void		SetIsSceneCamera			(const bool& set_to);

private:
	Frustum frustum;																						//

	Plane*	frustum_planes;																					// --- FRUSTUM CULLING
	float3* frustum_vertices;																				// -------------------
	
	uint	min_fov;																						// --- FRUSTUM SETTINGS 
	uint	max_fov;																						// --------------------

	bool	update_frustum_transform;																		// --- FRUSTUM UPDATES
	bool	update_projection_matrix;																		// -------------------

	bool	is_culling;																						// --- CAMERA FLAGS
	bool	in_orthogonal_view;																				//
	bool	hide_frustum;																					// ----------------

	bool	is_scene_camera;
};

#endif // !__CAMERA_H__
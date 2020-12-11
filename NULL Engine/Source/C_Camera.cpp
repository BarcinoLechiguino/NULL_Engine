#include "JSONParser.h"

#include "Application.h"
#include "M_Window.h"

#include "GameObject.h"
#include "C_Transform.h"

#include "C_Camera.h"

#define NUM_FRUSTUM_PLANES		6																			// A Frustum will ALWAYS be composed by 6 Planes.
#define NUM_FRUSTUM_VERTICES	8																			// As frustums are CUBOIDS, they will ALWAYS be composed by 8 Vertices.

C_Camera::C_Camera(GameObject* owner) : Component(owner, COMPONENT_TYPE::CAMERA),
frustum_planes		(nullptr),
frustum_vertices	(nullptr),
horizontal_fov		(90),
vertical_fov		(60),
min_fov				(60),
max_fov				(120),
near_plane_size		(float2::one),
far_plane_size		(float2::one),
culling				(false),
orthogonal_view		(false),
hide_frustum		(false),
is_scene_camera		(false)
{
	frustum_planes		= new Plane[NUM_FRUSTUM_PLANES];
	frustum_vertices	= new float3[NUM_FRUSTUM_VERTICES];

	InitFrustum();
}

C_Camera::~C_Camera()
{
	delete[] frustum_planes;
	delete[] frustum_vertices;
}

bool C_Camera::Update()
{
	bool ret = true;

	//frustum.AspectRatio();
	//frustum.SetHorizontalFovAndAspectRatio(60, frustum.AspectRatio());
	//frustum.ComputeProjectionMatrix();
	//frustum.ComputeViewMatrix();

	//frustum.SetHorizontalFovAndAspectRatio(90, (App->window->GetWidth() / App->window->GetHeight()));

	UpdateFrustum();

	GetViewMatrix();

	return ret;
}

bool C_Camera::CleanUp()
{
	bool ret = true;



	return ret;
}

bool C_Camera::SaveState(ParsonNode& root) const
{
	bool ret = true;

	root.SetNumber("Type", (uint)GetType());

	root.SetNumber("MinFOV", min_fov);
	root.SetNumber("MaxFOV", max_fov);

	root.SetBool("Culling", culling);
	root.SetBool("OrthogonalView", orthogonal_view);
	root.SetBool("HideFrustum", hide_frustum);

	root.SetBool("IsSceneCamera", is_scene_camera);

	return ret;
}

bool C_Camera::LoadState(ParsonNode& root)
{
	bool ret = true;

	min_fov				= root.GetNumber("MinFOV");
	max_fov				= root.GetNumber("MaxFOV");

	culling				= root.GetBool("Culling");
	orthogonal_view		= root.GetBool("OrthogonalView");
	hide_frustum		= root.GetBool("HideFrustum");

	is_scene_camera		= root.GetBool("IsSceneCamera");

	return ret;
}

// --- FRUSTUM METHODS ---
void C_Camera::InitFrustum()
{
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);

	frustum.SetPos(float3(0.0f, 0.0f, 0.0f));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	frustum.SetViewPlaneDistances(1.0f, 30.0f);
	frustum.SetPerspective(1.0f, 1.0f);

	//frustum.SetHorizontalFovAndAspectRatio(90.0f, frustum.AspectRatio());

	UpdateFrustumPlanes();
	UpdateFrustumVertices();
}

void C_Camera::UpdateFrustum()
{
	float4x4 world_transform	= this->GetOwner()->GetTransformComponent()->GetWorldTransform();
	float3x4 world_matrix		= float3x4::identity;

	world_matrix.SetTranslatePart(world_transform.TranslatePart());
	world_matrix.SetRotatePart(world_transform.RotatePart());
	world_matrix.Scale(world_transform.GetScale());

	frustum.SetWorldMatrix(world_matrix);

	UpdateFrustumPlanes();
	UpdateFrustumVertices();
}

void C_Camera::UpdateFrustumPlanes()
{
	frustum.GetPlanes(frustum_planes);
}

void C_Camera::UpdateFrustumVertices()
{
	frustum.GetCornerPoints(frustum_vertices);
}

Plane* C_Camera::GetFrustumPlanes() const
{
	return frustum_planes;
}

float3* C_Camera::GetFrustumVertices() const
{
	return frustum_vertices;
}

float C_Camera::ComputeAspectRatio(const uint& horizontal_fov, const uint& vertical_fov)
{
	float width		= Tan((float)horizontal_fov / 2);
	float height	= Tan((float)vertical_fov / 2);
	
	return (width / height);
}

float3x4 C_Camera::GetViewMatrix()
{
	return frustum.ComputeViewMatrix();
}

float4x4 C_Camera::GetProjectionMatrix()
{
	return frustum.ComputeProjectionMatrix();
}

bool C_Camera::FrustumCointainsAABB(const AABB& aabb) const
{
	float3 aabb_vertices[8];
	aabb.GetCornerPoints(aabb_vertices);

	uint planes_containing_all_verts = 0;																	// Can be used to check whether or not the geometric figure is intersected.

	for (uint p = 0; p < 6; ++p)
	{
		bool all_verts_are_in	= true;
		uint verts_in_plane		= 8;

		for (uint v = 0; v < 8; ++v)
		{
			if (frustum_planes[p].IsOnPositiveSide(aabb_vertices[v]))										// IsOnPositiveSide() returns true if the given point is behind the frustum plane.
			{
				all_verts_are_in = false;
				--verts_in_plane;
			}
		}

		if (verts_in_plane == 0)
		{
			return false;
		}

		if (all_verts_are_in)
		{
			++planes_containing_all_verts;
		}
	}

	if (planes_containing_all_verts == 6)
	{
		return true;
	}

	return false;
}

bool C_Camera::FrustumIntersectsAABB(const AABB& aabb) const
{
	float3 aabb_vertices[8];
	aabb.GetCornerPoints(aabb_vertices);

	for (uint p = 0; p < 6; ++p)
	{
		uint verts_in_plane = 8;

		for (uint v = 0; v < 8; ++v)
		{
			if (frustum_planes[p].IsOnPositiveSide(aabb_vertices[v]))
			{
				--verts_in_plane;
			}
		}

		if (verts_in_plane == 0)
		{
			return false;
		}
	}

	return true;
}

/*bool C_Camera::FrustumIntersectsAABB(const AABB& aabb) const
{
	float3 aabb_vertices[8];
	aabb.GetCornerPoints(aabb_vertices);
	
	uint total_in = 0;

	for (uint p = 0; p < 6; ++p)
	{
		uint verts_in_plane		= 8;
		bool all_verts_are_in	= true;

		for (uint v = 0; v < 8; ++v)
		{
			if (frustum_planes[p].IsOnPositiveSide(aabb_vertices[v]))
			{
				all_verts_are_in = false;
				--verts_in_plane;
			}
		}

		if (verts_in_plane == 0)																			// If no verts are inside a plane, then the aabb is completely out of the frustum.
		{
			return CULLING_STATE::OUT;																		// RETURN OUT;
		}

		total_in += (all_verts_are_in) ? 1 : 0;																// If all verts are inside the plane, then the plane will be validated.
	}

	if (total_in == NUM_FRUSTUM_PLANES)
	{
		return CULLING_STATE::IN;																			// RETURN IN;
	}

	return CULLING_STATE::INTERSECTED;																		// RETURN INTERSECT;
}*/

bool C_Camera::IsCulling() const
{
	return culling;
}

bool C_Camera::OrthogonalView() const
{
	return orthogonal_view;
}

bool C_Camera::FrustumIsHidden() const
{
	return hide_frustum;
}

void C_Camera::SetIsCulling(const bool& set_to)
{
	culling = set_to;
}

void C_Camera::SetOrthogonalView(const bool& set_to)
{
	orthogonal_view = set_to;

	if (orthogonal_view)
	{
		// SET TO ORTHOGONAL
		// --- Near plane size = Far plane size
		// --- Update View Matrix
	}
}

void C_Camera::SetFrustumIsHidden(const bool& set_to)
{
	hide_frustum = set_to;
}

bool C_Camera::IsSceneCamera() const
{
	return is_scene_camera;
}

void C_Camera::SetIsSceneCamera(const bool& set_to)
{
	is_scene_camera = set_to;
}
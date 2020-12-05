#include "Configuration.h"

#include "Application.h"
#include "M_Renderer3D.h"

#include "R_Mesh.h"

#include "GameObject.h"
#include "C_Transform.h"
#include "C_Material.h"

#include "C_Mesh.h"

C_Mesh::C_Mesh(GameObject* owner) : Component(owner, COMPONENT_TYPE::MESH),
mesh	(nullptr)
{

}

C_Mesh::~C_Mesh()
{

}

bool C_Mesh::Update()
{
	bool ret = true;

	if (IsActive())
	{
		if (mesh != nullptr)
		{
			Render();
		}
	}

	return ret;
}

bool C_Mesh::CleanUp()
{
	bool ret = true;

	if (mesh != nullptr)
	{
		mesh->CleanUp();

		RELEASE(mesh);
	}

	return ret;
}

bool C_Mesh::SaveConfiguration(ParsonNode& configuration) const
{
	bool ret = true;

	return ret;
}

bool C_Mesh::LoadConfiguration(ParsonNode& configuration)
{
	bool ret = true;

	return ret;
}

// --- C_MESH METHODS ---
bool C_Mesh::Render()
{
	bool ret = true;

	C_Transform* c_transform = owner->GetTransformComponent();
	C_Material* c_material = owner->GetMaterialComponent();

	App->renderer->RenderMesh(c_transform->GetWorldTransform(), this, c_material);

	return ret;
}

R_Mesh* C_Mesh::GetMesh() const
{
	return mesh;
}

void C_Mesh::SetMesh(R_Mesh* r_mesh)
{
	mesh = r_mesh;
}

const char* C_Mesh::GetMeshPath() const
{
	if (mesh != nullptr)
	{
		return mesh->GetAssetsPath();
	}

	return "NONE";
}

void C_Mesh::SetMeshPath(const char* path)
{
	if (mesh != nullptr)
	{
		mesh->SetAssetsPath(path);
	}
}

void C_Mesh::GetMeshData(uint& num_vertices, uint& num_normals, uint& num_tex_coords, uint& num_indices)
{
	if (mesh != nullptr)
	{
		num_vertices	= mesh->vertices.size();
		num_normals		= mesh->normals.size();
		num_tex_coords	= mesh->tex_coords.size();
		num_indices		= mesh->indices.size();
	}
}

bool C_Mesh::GetDrawVertexNormals() const
{
	if (mesh != nullptr)
	{
		return mesh->draw_vertex_normals;
	}

	return false;
}

bool C_Mesh::GetDrawFaceNormals() const
{
	if (mesh != nullptr)
	{
		return mesh->draw_face_normals;
	}

	return false;
}

void C_Mesh::SetDrawVertexNormals(const bool& set_to)
{
	if (mesh != nullptr)
	{
		mesh->draw_vertex_normals = set_to;
	}
}

void C_Mesh::SetDrawFaceNormals(const bool& set_to)
{
	if (mesh != nullptr)
	{
		mesh->draw_face_normals = set_to;
	}
}
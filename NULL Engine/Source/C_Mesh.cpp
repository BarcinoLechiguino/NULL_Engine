#include "Application.h"
#include "M_Renderer3D.h"
#include "R_Mesh.h"

#include "GameObject.h"

#include "C_Mesh.h"

C_Mesh::C_Mesh(GameObject* owner) : Component(owner, COMPONENT_TYPE::MESH, "Mesh"), mesh(nullptr)
{

}

C_Mesh::~C_Mesh()
{

}

bool C_Mesh::Update()
{
	bool ret = true;

	if (mesh != nullptr)
	{
		App->renderer->DrawMesh(mesh);
	}

	return ret;
}

bool C_Mesh::CleanUp()
{
	bool ret = true;

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

std::string C_Mesh::GetMeshPath() const
{
	if (mesh != nullptr)
	{
		return mesh->path;
	}

	return "NONE";
}

void C_Mesh::SetMeshPath(const char* path)
{
	if (mesh != nullptr)
	{
		mesh->path = path;
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

bool C_Mesh::GetDrawNormals() const
{
	if (mesh != nullptr)
	{
		return mesh->draw_normals;
	}

	return false;
}

void C_Mesh::SetDrawNormals(const bool& set_to)
{
	if (mesh != nullptr)
	{
		mesh->draw_normals = set_to;
	}
}
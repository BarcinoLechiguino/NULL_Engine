#include "Application.h"
#include "M_Renderer3D.h"
#include "R_Mesh.h"

#include "GameObject.h"
#include "C_Transform.h"
#include "C_Material.h"

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
		Render();
	}

	return ret;
}

bool C_Mesh::CleanUp()
{
	bool ret = true;

	if (mesh != nullptr)
	{
		mesh->CleanUp();

		delete mesh;
		mesh = nullptr;
	}

	return ret;
}

// ------ C_MESH METHODS ------
bool C_Mesh::Render()
{
	bool ret = true;

	C_Transform* transform	= owner->GetTransformComponent();
	C_Material* material	= owner->GetMaterialComponent();

	uint tex_id				= -1;
	bool tex_is_active		= true;

	if (material != nullptr)
	{
		if (material->IsActive())
		{
			if (material->GetTexture() != nullptr)
			{
				if (material->UseDefaultTexture())
				{
					tex_id = 0;
				}
				else
				{
					tex_id = material->GetTextureId();
				}
			}
		}
		else
		{
			tex_is_active = false;
		}
	}

	if (this->IsActive())																				// Added this-> to further clarify that IsActive() refers to this C_Mesh component.
	{
		if (mesh != nullptr)
		{
			App->renderer->RenderMesh(transform->GetWorldTransform(), mesh, tex_id, tex_is_active);
		}
	}

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
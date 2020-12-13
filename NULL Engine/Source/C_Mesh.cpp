#include "JSONParser.h"

#include "Application.h"
#include "M_Renderer3D.h"
#include "M_ResourceManager.h"

#include "R_Mesh.h"

#include "GameObject.h"
#include "C_Transform.h"
#include "C_Material.h"

#include "C_Mesh.h"

C_Mesh::C_Mesh(GameObject* owner) : Component(owner, COMPONENT_TYPE::MESH),
r_mesh				(nullptr),
show_wireframe		(false),
show_bounding_box	(false)
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
		if (r_mesh != nullptr)
		{
			//Render();
		}
	}

	return ret;
}

bool C_Mesh::CleanUp()
{
	bool ret = true;

	r_mesh = nullptr;												// Resources are now taken care by M_ResourceManager;

	return ret;
}

bool C_Mesh::SaveState(ParsonNode& root) const
{
	bool ret = true;

	if (r_mesh != nullptr)
	{
		root.SetNumber("Type", (uint)GetType());
		root.SetNumber("UID", r_mesh->GetUID());
		root.SetString("Name", r_mesh->GetAssetsFile());
		root.SetString("Path", r_mesh->GetLibraryPath());
		root.SetString("File", r_mesh->GetLibraryFile());
		root.SetBool("ShowWireframe", show_wireframe);
		root.SetBool("ShowBoundingBox", show_bounding_box);
	}

	return ret;
}

bool C_Mesh::LoadState(ParsonNode& root)
{
	bool ret = true;

	r_mesh = nullptr;

	r_mesh				= (R_Mesh*)App->resource_manager->GetResource((uint)root.GetNumber("UID"));
	show_wireframe		= root.GetBool("ShowWireframe");
	show_bounding_box	= root.GetBool("ShowBoundingBox");

	if (r_mesh == nullptr)
	{
		r_mesh = (R_Mesh*)App->resource_manager->GetResourceFromFile(root.GetString("File"));

		if (r_mesh == nullptr)
		{
			LOG("[ERROR] Loading Scene: Could not find Mesh %s with UID: %u! Try reimporting the model.", root.GetString("File"), (uint)root.GetNumber("UID"));
		}
	}

	return ret;
}

// --- C_MESH METHODS ---
bool C_Mesh::Render()
{
	bool ret = true;

	/*C_Transform* c_transform	= owner->GetTransformComponent();
	C_Material* c_material		= owner->GetMaterialComponent();

	App->renderer->RenderMesh(c_transform->GetWorldTransform(), this, c_material);*/

	return ret;
}

R_Mesh* C_Mesh::GetMesh() const
{
	return r_mesh;
}

void C_Mesh::SetMesh(R_Mesh* r_mesh)
{
	this->r_mesh = r_mesh;
}

const char* C_Mesh::GetMeshPath() const
{
	if (r_mesh != nullptr)
	{
		return r_mesh->GetAssetsPath();
	}

	return "NONE";
}

const char* C_Mesh::GetMeshFile() const
{
	if (r_mesh != nullptr)
	{
		return r_mesh->GetAssetsFile();
	}

	return "NONE";
}

void C_Mesh::SetMeshPath(const char* path)
{
	if (r_mesh != nullptr)
	{
		r_mesh->SetAssetsPath(path);
	}
}

void C_Mesh::GetMeshData(uint& num_vertices, uint& num_normals, uint& num_tex_coords, uint& num_indices)
{
	if (r_mesh != nullptr)
	{
		num_vertices	= r_mesh->vertices.size();
		num_normals		= r_mesh->normals.size();
		num_tex_coords	= r_mesh->tex_coords.size();
		num_indices		= r_mesh->indices.size();
	}
}

bool C_Mesh::GetDrawVertexNormals() const
{
	if (r_mesh != nullptr)
	{
		return r_mesh->draw_vertex_normals;
	}

	return false;
}

bool C_Mesh::GetDrawFaceNormals() const
{
	if (r_mesh != nullptr)
	{
		return r_mesh->draw_face_normals;
	}

	return false;
}

void C_Mesh::SetDrawVertexNormals(const bool& set_to)
{
	if (r_mesh != nullptr)
	{
		r_mesh->draw_vertex_normals = set_to;
	}
}

void C_Mesh::SetDrawFaceNormals(const bool& set_to)
{
	if (r_mesh != nullptr)
	{
		r_mesh->draw_face_normals = set_to;
	}
}

void C_Mesh::GetBoundingBoxVertices(math::float3* bb_vertices) const
{
	if (r_mesh != nullptr)
	{
		r_mesh->aabb.GetCornerPoints(bb_vertices);
	}
}

bool C_Mesh::GetShowWireframe() const
{
	return show_wireframe;
}

void C_Mesh::SetShowWireframe(const bool& set_to)
{
	show_wireframe = set_to;
}

bool C_Mesh::GetShowBoundingBox() const
{
	return show_bounding_box;
}

void C_Mesh::SetShowBoundingBox(const bool& set_to)
{
	show_bounding_box = set_to;
}
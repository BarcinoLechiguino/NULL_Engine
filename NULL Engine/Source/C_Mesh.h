#ifndef __C_MESH_H__
#define __C_MESH_H__

#include "Component.h"

class GameObject;
class R_Mesh;

class C_Mesh : public Component
{
public:
	C_Mesh(GameObject* owner);
	~C_Mesh();

	bool Update() override;
	bool CleanUp() override;

public:
	R_Mesh* GetMesh() const;
	void SetMesh(R_Mesh* r_mesh);

	std::string GetMeshPath() const;
	void SetMeshPath(const char* path);

	void GetMeshData(uint& num_vertices, uint& num_normals, uint& num_tex_coords, uint& num_indices);

	bool GetDrawNormals() const;
	void SetDrawNormals(const bool& set_to);

private:
	R_Mesh* mesh;
};

#endif // !__C_MESH_H__
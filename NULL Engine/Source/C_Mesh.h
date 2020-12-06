#ifndef __C_MESH_H__
#define __C_MESH_H__

#include "Component.h"

class ParsonNode;
class GameObject;
class R_Mesh;

class C_Mesh : public Component
{
public:
	C_Mesh(GameObject* owner);
	~C_Mesh();

	bool Update		() override;
	bool CleanUp	() override;

	bool SaveState	(ParsonNode& root) const override;
	bool LoadState	(ParsonNode& root) override;

public:
	bool			Render					();										// Will render the mesh along with the corresponding textures and transform.

	R_Mesh*			GetMesh					() const;								// Returns the R_Mesh* variable of the component. If there is no mesh the default value will be nullptr.
	void			SetMesh					(R_Mesh* r_mesh);						// Sets the R_Mesh* variable of the component. Will be normally set when a model is imported.

	const char*		GetMeshPath				() const;
	void			SetMeshPath				(const char* path);

	void			GetMeshData				(uint& num_vertices, uint& num_normals, uint& num_tex_coords, uint& num_indices);

	bool			GetDrawVertexNormals	() const;
	bool			GetDrawFaceNormals		() const;
	void			SetDrawVertexNormals	(const bool& set_to);
	void			SetDrawFaceNormals		(const bool& set_to);

private:
	R_Mesh* r_mesh;
};

#endif // !__C_MESH_H__
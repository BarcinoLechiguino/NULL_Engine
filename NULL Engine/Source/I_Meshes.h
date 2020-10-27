#ifndef __I_MESHES_H__
#define __I_MESHES_H__

struct aiScene;
struct aiMesh;

class R_Mesh;

namespace Importer
{
	namespace Meshes
	{
		R_Mesh*	Create();																	// Generates an empty R_Mesh resource. Will use R_Mesh's default constructor.

		void	Import(const aiScene* ai_scene, const aiMesh* ai_mesh, R_Mesh* r_mesh);		// Processes aiMesh data into a ready-to-use R_Mesh to be saved later. Will return nullptr on ERROR.

		uint64	Save(const R_Mesh* mesh, char** buffer);									// Processes R_Mesh data into a buffer ready to save. Will return buffer file's size (0 on ERROR).

		void	Load(const char* buffer, R_Mesh* mesh);										// Processes buffer data into a ready-to-use R_Mesh. Will return nullptr on ERROR.
	}
}

#endif // !__I_MESHES_H__
#ifndef __I_MESHES_H__
#define __I_MESHES_H__

struct aiMesh;

class R_Mesh;

namespace Importer
{
	namespace Meshes
	{
		R_Mesh*	Create();														// Generates/Instances an empty R_Mesh resource. Will use R_Mesh's default constructor.

		void	Import(const aiMesh* imported_mesh, R_Mesh* resource_mesh);		// Processes aiMesh data into a ready-to-use R_Mesh to be saved later. Will return nullptr on ERROR.

		uint64	Save(const R_Mesh* mesh, char** buffer);						// Processes R_Mesh data into a buffer ready to save. Will return the size of the buffer file (0 on ERROR).

		void	Load(const char* buffer, R_Mesh* mesh);							// Processes buffer data into a ready-to-use R_Mesh. Will return nullptr on ERROR.
	}
}

#endif // !__I_MESHES_H__
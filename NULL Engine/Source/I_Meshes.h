#ifndef __I_MESHES_H__
#define __I_MESHES_H__

#include <vector>

#include "Assimp.h"

struct aiScene;
struct aiNode;
struct aiMesh;

class R_Mesh;

typedef unsigned __int64 uint64;

enum class HEADER_DATA
{
	VERTICES,
	NORMALS,
	TEX_COORDS,
	INDICES,
	VBO,
	NBO,
	TBO,
	IBO
};

namespace Importer
{
	namespace Meshes
	{
		void	Import	(const aiMesh* ai_mesh, R_Mesh* r_mesh);											// Loads all mesh data from the given aiNode & stores it in a new R_Mesh.

		uint64	Save	(const R_Mesh* r_mesh, char** buffer);												// Processes R_Mesh data into ready-to-save buffer. Will return buffer file's size.

		bool	Load	(const char* buffer, R_Mesh* r_mesh);												// Processes buffer data into a ready-to-use R_Mesh. Will return nullptr on ERROR.

		namespace Utilities
		{
			void ImportMeshData	(const aiMesh* ai_mesh, R_Mesh* r_mesh);									// Stores the data of the given aiMesh* in the given R_Mesh*.

			void GetVertices	(const aiMesh* ai_mesh, R_Mesh* r_mesh, uint size);							// Gets the data for the meshes' vertex positions.
			void GetNormals		(const aiMesh* ai_mesh, R_Mesh* r_mesh, uint size);							// Gets the data for the meshes' vertex normals.
			void GetTexCoords	(const aiMesh* ai_mesh, R_Mesh* r_mesh, uint size);							// Gets the data for the meshes' vertex tex coords.
			void GetIndices		(const aiMesh* ai_mesh, R_Mesh* r_mesh, uint size);							// Gets the data for the meshes' indices.
		}
	}
}

#endif // !__I_MESHES_H__
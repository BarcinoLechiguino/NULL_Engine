#ifndef __I_MESHES_H__
#define __I_MESHES_H__

#include <vector>

#include "Assimp.h"

struct aiScene;
struct aiNode;
struct aiMesh;

class R_Mesh;

namespace Importer
{
	namespace Meshes
	{
		void	Import(const aiScene* scene, const aiNode* node, std::vector<R_Mesh*>& meshes);				// Loads all mesh data from the given aiNode & stores the new r_meshes in a vector.
		void	Import(const aiMesh* ai_mesh, R_Mesh* r_mesh);												// Loads all mesh data from the given aiNode & stores the new r_meshes in a vector.

		uint64	Save(const R_Mesh* mesh, char** buffer);													// Processes R_Mesh data into ready-to-save buffer. Will return buffer file's size.

		bool	Load(const char* buffer, R_Mesh* mesh);														// Processes buffer data into a ready-to-use R_Mesh. Will return nullptr on ERROR.

		namespace Utilities
		{
			void ImportMeshData(const aiMesh* ai_mesh, R_Mesh* r_mesh);										// Stores the data of the given aiMesh* in the given R_Mesh*.

			void GetVertices(const aiMesh* ai_mesh, R_Mesh* r_mesh, uint size);								// Gets the data for the meshes' vertex positions.
			void GetNormals(const aiMesh* ai_mesh, R_Mesh* r_mesh, uint size);								// Gets the data for the meshes' vertex normals.
			void GetTexCoords(const aiMesh* ai_mesh, R_Mesh* r_mesh, uint size);							// Gets the data for the meshes' vertex tex coords.
			void GetIndices(const aiMesh* ai_mesh, R_Mesh* r_mesh, uint size);								// Gets the data for the meshes' indices.
		}
	}
}

#endif // !__I_MESHES_H__
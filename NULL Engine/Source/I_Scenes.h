#ifndef __I_SCENES_H__
#define __I_SCENES_H__

#include <vector>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;

class R_Mesh;

class GameObject;

namespace Importer
{
	namespace Scenes	// Maybe call it Models? Are Models a separate thing?
	{
		void	Import(const char* path, std::vector<GameObject*>& game_objects);

		uint64	Save(const aiScene* ai_scene, char** buffer);

		void	Load(const char* buffer, aiScene* ai_scene);

		namespace Utilities
		{
			void			ImportFromAssets(const char* path, std::vector<GameObject*>& game_objects);
			void			ImportFromLibrary(const char* path, std::vector<GameObject*>& game_objects);
			
			void			ProcessNode(const char* scene_path, const aiScene* ai_scene, const aiNode* ai_node, std::vector<GameObject*>& game_objects, GameObject* parent);
			
			const aiNode*	ImportTransform(const aiNode* ai_node, GameObject* game_object);
			void			ImportMeshesAndMaterials(const char* path, const aiScene* ai_scene, const aiNode* ai_node, GameObject* game_object);
			void			ImportMesh(const char* path, const aiMesh* ai_mesh, GameObject* game_object);
			void			ImportMaterial(const char* path, const aiMaterial* ai_material, GameObject* game_object);

			bool			NodeIsDummyNode(const aiNode& ai_node);
		}
	}
}

#endif // !__I_SCENES_H__
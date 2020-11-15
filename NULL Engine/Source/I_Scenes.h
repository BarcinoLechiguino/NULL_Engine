#ifndef __I_SCENES_H__
#define __I_SCENES_H__

#include <vector>

struct aiScene;
struct aiNode;

class GameObject;

namespace Importer
{
	namespace Scenes	// Maybe call it Models?
	{
		void Import(const aiScene* ai_scene);
		void Import(const char* path, std::vector<GameObject*>& game_object_nodes);

		void Save(const aiScene* ai_scene, char** buffer);

		void Load(const char* buffer, aiScene* ai_scene);

		namespace Utilities
		{
			void ProcessNode(const aiScene* ai_scene, const aiNode* ai_node, std::vector<GameObject*>& game_object_nodes);
			
			void ImportTransform(const aiNode* ai_node, GameObject* game_object_node);
			void ImportMeshes(const aiNode* ai_node, GameObject* game_object_node);
		}
	}
}

#endif // !__I_SCENES_H__
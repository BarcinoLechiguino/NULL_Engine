#ifndef __I_SCENES_H__
#define __I_SCENES_H__

#include <vector>

struct aiScene;
struct aiNode;

class R_Mesh;

class GameObject;

namespace Importer
{
	namespace Scenes	// Maybe call it Models? Are Models a separate thing?
	{
		void Import(const char* path, std::vector<GameObject*>& game_object_nodes);

		void Save(const aiScene* ai_scene, char** buffer);

		void Load(const char* buffer, aiScene* ai_scene);

		namespace Utilities
		{
			void ProcessNode(const char* scene_path, const aiScene* ai_scene, const aiNode* ai_node, std::vector<GameObject*>& game_objects, GameObject* parent);
			
			void ImportTransform(const aiNode* ai_node, GameObject* game_object);
			void ImportMeshes(const char* scene_file, const aiScene* ai_scene, const aiNode* ai_node, GameObject* game_object);
			void ImportMaterials(const char* scene_path, GameObject* game_object);
		}
	}
}

#endif // !__I_SCENES_H__
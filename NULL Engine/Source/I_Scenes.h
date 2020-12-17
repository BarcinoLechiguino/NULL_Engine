#ifndef __I_SCENES_H__
#define __I_SCENES_H__

#include <vector>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;
struct ModelNode;

class R_Model;

namespace Importer
{
	namespace Scenes	// Maybe call it Models? Are Models a separate thing?
	{
		void	Import	(const char* path, R_Model* r_model);
		uint64	Save	(const R_Model* r_model, char** buffer);
		void	Load	(const char* buffer, R_Model* r_model);

		namespace Utilities
		{
			void			ImportFromAssets			(const char* path, R_Model* r_model);
			void			ImportFromLibrary			(const char* path, R_Model* r_model);
			void			ProcessNode					(const char* scene_path, const aiScene* ai_scene, const aiNode* ai_node, R_Model* r_model, const ModelNode& parent);
			
			const aiNode*	ImportTransform				(const aiNode* ai_node, ModelNode& model_node);
			void			ImportMeshesAndMaterials	(const char* path, const aiScene* ai_scene, const aiNode* ai_node, ModelNode& model_node);
			void			ImportMesh					(const char* path, const char* name, const aiMesh* ai_mesh, ModelNode& model_node);
			void			ImportMaterial				(const char* path, const char* name, const aiMaterial* ai_material, ModelNode& model_node);

			bool			NodeIsDummyNode				(const aiNode& ai_node);
		}
	}
}

#endif // !__I_SCENES_H__
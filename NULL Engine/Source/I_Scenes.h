#ifndef __I_SCENES_H__
#define __I_SCENES_H__

#include <vector>
#include <map>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;

struct ModelNode;
struct Material;

class R_Model;

typedef unsigned int		uint;
typedef unsigned __int64	uint64;

namespace Importer
{
	namespace Scenes
	{
		void	Import	(const char* buffer, uint size, R_Model* r_model);
		uint64	Save	(const R_Model* r_model, char** buffer);
		void	Load	(const char* buffer, R_Model* r_model);

		namespace Utilities
		{
			void			ProcessNode					(const aiScene* ai_scene, const aiNode* ai_node, R_Model* r_model, const ModelNode& parent);
			
			const aiNode*	ImportTransform				(const aiNode* ai_node, ModelNode& model_node);
			void			ImportMeshesAndMaterials	(const aiScene* ai_scene, const aiNode* ai_node, R_Model* r_model, ModelNode& model_node);
			
			void			ImportMesh					(const char* node_name, const aiMesh* ai_mesh, ModelNode& model_node);
			void			ImportMaterial				(const char* node_name, const aiMaterial* ai_material, R_Model* r_model, ModelNode& model_node);
			void			ImportTexture				(std::vector<Material> materials, ModelNode& model_node);

			bool			NodeIsDummyNode				(const aiNode& ai_node);

			static std::vector<aiMesh*>			ai_meshes;
			static std::vector<aiMaterial*>		ai_materials;
			static std::map<uint, ModelNode>	loaded_nodes;
		}
	}
}

#endif // !__I_SCENES_H__
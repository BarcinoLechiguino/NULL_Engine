#ifndef __R_MODEL_H__
#define __R_MODEL_H__

#include "Resource.h"

struct	aiScene;
struct	aiNode;
struct	aiMesh;
struct	aiMaterial;
enum	aiTextureType;

struct	Texture;
class	R_Mesh;
//class R_Texture;

class R_Model : public Resource
{
public:
	R_Model(std::string full_path, vec4 colour = vec4(1.0f, 1.0f, 1.0f, 1.0f));
	~R_Model();

	void Draw();
	void DrawNormals();
	void DrawTexCoords();

	void SetMaterialColour(vec4 colour);

	void ProcessScene(const aiScene* scene);

private:
	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterialTextures(aiMaterial* material, aiTextureType type, R_Mesh* r_mesh);

private:
	std::vector<R_Mesh*> meshes;
	std::vector<Texture*> loaded_textures;
	
	std::string full_path;
	std::string directory;

	vec4 colour;

};

#endif // !__R_MODEL_H__
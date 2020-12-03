#ifndef __R_MODEL_H__
#define __R_MODEL_H__

#include "MathGeoLib/include/Math/float4.h"
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
	R_Model(std::string full_path, float4 colour = float4::one);
	~R_Model();

	void Draw();
	void DrawNormals();

	void SetMaterialColour(float4 colour);

	void ProcessScene(const aiScene* scene);

private:
	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterialTextures(aiMaterial* material, aiTextureType type, R_Mesh* r_mesh);

public:
	std::vector<R_Mesh*> meshes;

private:
	std::vector<Texture*> loaded_textures;
	
	std::string full_path;
	std::string directory;

	float4 colour;

};

#endif // !__R_MODEL_H__
#ifndef __R_MODEL_H__
#define __R_MODEL_H__

#include <string>

#include "MathGeoTransform.h"
#include "Resource.h"

typedef unsigned __int32 uint32;

struct ModelNode
{
	ModelNode();
	ModelNode(const char* name, uint32 UID, uint32 parent_UID, Transform transform, uint32 mesh_UID, uint32 material_UID, uint32 texture_UID);

	std::string name;																										// Name of the Model Node.
	uint32		uid;																										// UID of this Model Node.
	uint32		parent_uid;																									// UID of the parent Model Node.
	Transform	transform;																									// Transform of the Model Node.
	uint32		mesh_uid;																									// UID of the Mesh Resource (R_Mesh) of the Model Node.
	uint32		material_uid;																								// UID of the Material Resource (R_Material) of the Model Node.
	uint32		texture_uid;																								// UID of the Texture Resource (R_Texture) of the Model Node.
};

class R_Model : public Resource
{
public:
	R_Model();
	~R_Model();



public:
	std::vector<ModelNode> model_nodes;
};

#endif // !__R_MODEL_H__
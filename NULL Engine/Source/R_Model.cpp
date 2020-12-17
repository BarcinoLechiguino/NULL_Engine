#include "R_Model.h"

R_Model::R_Model() : Resource(RESOURCE_TYPE::NONE)
{

}

R_Model::~R_Model()
{

}

ModelNode::ModelNode() :
name			("[NONE]"),
uid				(0),
parent_uid		(0),
transform		(Transform()),
mesh_uid		(0),
material_uid	(0),
texture_uid		(0)
{

}

ModelNode::ModelNode(const char* name, uint32 UID, uint32 parent_UID, Transform transform, uint32 mesh_UID, uint32 material_UID, uint32 texture_UID) :
name			(name),
uid				(UID),
parent_uid		(parent_UID),
transform		(transform),
mesh_uid		(mesh_UID),
material_uid	(material_UID),
texture_uid		(texture_UID)
{

}
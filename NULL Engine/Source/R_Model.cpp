#include "VariableTypedefs.h"

#include "JSONParser.h"

#include "R_Model.h"

R_Model::R_Model() : Resource(RESOURCE_TYPE::NONE)
{

}

R_Model::~R_Model()
{

}

bool R_Model::CleanUp()
{
	bool ret = true;

	model_nodes.clear();

	return ret;
}

bool R_Model::SaveMeta(ParsonNode& meta_root) const
{
	bool ret = true;



	return ret;
}

bool R_Model::LoadMeta(const ParsonNode& meta_root)
{
	bool ret = true;



	return ret;
}


// --- MODEL NODE METHODS ---
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

bool ModelNode::Save(ParsonNode& root) const
{
	bool ret = true;

	root.SetString("Name", name.c_str());

	root.SetNumber("UID", uid);
	root.SetNumber("ParentUID", parent_uid);

	ParsonNode transform_node	= root.SetNode("Transform");
	ParsonArray position		= transform_node.SetArray("LocalPosition");
	ParsonArray rotation		= transform_node.SetArray("LocalRotation");
	ParsonArray scale			= transform_node.SetArray("LocalScale");

	position.SetFloat3(transform.position);
	rotation.SetFloat4(transform.rotation.CastToFloat4());
	scale.SetFloat3(transform.scale);

	root.SetNumber("MeshUID", mesh_uid);
	root.SetNumber("MaterialUID", material_uid);
	root.SetNumber("TextureUID", texture_uid);

	return ret;
}

bool ModelNode::Load(const ParsonNode& root)
{
	bool ret = true;

	name = root.GetString("Name");

	uid				= (uint32)root.GetNumber("UID");
	parent_uid		= (uint32)root.GetNumber("ParentUID");

	ParsonNode transform_node	= root.GetNode("Transform");
	ParsonArray position		= root.GetArray("LocalPosition");
	ParsonArray rotation		= root.GetArray("LocalRotation");
	ParsonArray scale			= root.GetArray("LocalScale");

	position.GetFloat3(0, transform.position);
	rotation.GetFloat4(0, transform.rotation);
	scale.GetFloat3(0, transform.position);

	mesh_uid		= (uint32)root.GetNumber("MeshUID");
	material_uid	= (uint32)root.GetNumber("MaterialUID");
	texture_uid		= (uint32)root.GetNumber("TextureUID");

	return ret;
}

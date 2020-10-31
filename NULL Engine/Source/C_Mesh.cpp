#include "GameObject.h"

#include "C_Mesh.h"

C_Mesh::C_Mesh(GameObject* owner) : Component(owner, COMPONENT_TYPE::MESH, "Mesh")
{

}

C_Mesh::~C_Mesh()
{

}

bool C_Mesh::Update()
{
	bool ret = true;

	return ret;
}

bool C_Mesh::CleanUp()
{
	bool ret = true;

	return ret;
}
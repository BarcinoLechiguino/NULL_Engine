#include "GameObject.h"

#include "C_Material.h"

C_Material::C_Material(GameObject* owner) : Component(owner, COMPONENT_TYPE::MATERIAL, "Material")
{

}

C_Material::~C_Material()
{

}

bool C_Material::Update()
{
	bool ret = true;

	return ret;
}

bool C_Material::CleanUp()
{
	bool ret = true;

	return ret;
}
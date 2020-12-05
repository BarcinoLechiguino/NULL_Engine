#include "Configuration.h"

#include "GameObject.h"

#include "C_Light.h"

C_Light::C_Light(GameObject* owner) : Component(owner, COMPONENT_TYPE::LIGHT)
{

}

C_Light::~C_Light()
{

}

bool C_Light::Update()
{
	bool ret = true;

	return ret;
}

bool C_Light::CleanUp()
{
	bool ret = true;

	return ret;
}

bool C_Light::SaveConfiguration(ParsonNode& configuration) const
{
	bool ret = true;



	return ret;
}

bool C_Light::LoadConfiguration(ParsonNode& configuration)
{
	bool ret = true;



	return ret;
}
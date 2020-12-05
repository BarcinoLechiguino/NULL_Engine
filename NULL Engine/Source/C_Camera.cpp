#include "Configuration.h"

#include "GameObject.h"

#include "C_Camera.h"

C_Camera::C_Camera(GameObject* owner) : Component(owner, COMPONENT_TYPE::CAMERA)
{

}

C_Camera::~C_Camera()
{

}

bool C_Camera::Update()
{
	bool ret = true;



	return ret;
}

bool C_Camera::CleanUp()
{
	bool ret = true;



	return ret;
}

bool C_Camera::SaveConfiguration(ParsonNode& configuration) const
{
	bool ret = true;

	return ret;
}

bool C_Camera::LoadConfiguration(ParsonNode& configuration)
{
	bool ret = true;

	return ret;
}
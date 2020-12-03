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
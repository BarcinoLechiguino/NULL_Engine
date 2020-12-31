#include "GameObject.h"

#include "C_Animation.h"

C_Animation::C_Animation(GameObject* owner) : Component(owner, COMPONENT_TYPE::ANIMATION)
{

}

C_Animation::~C_Animation()
{

}

bool C_Animation::Update()
{
	bool ret = true;



	return ret;
}

bool C_Animation::CleanUp()
{
	bool ret = true;



	return ret;
}

bool C_Animation::SaveState(ParsonNode& root) const
{
	bool ret = true;



	return ret;
}

bool C_Animation::LoadState(ParsonNode& root)
{
	bool ret = true;



	return ret;
}

// --- C_ANIMATION METHODS ---

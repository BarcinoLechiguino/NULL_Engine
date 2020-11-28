#include "Random.h"

#include "Resource.h"

Resource::Resource() : id(0)
{
	SetID();
}

Resource::~Resource()
{

}

bool Resource::CleanUp()
{
	bool ret = true;



	return ret;
}

// --- RESOURCE METHODS ---
uint Resource::GetID()
{
	return id;
}

void Resource::SetID()
{
	id = Random::LCG::GetRandomUint();
}
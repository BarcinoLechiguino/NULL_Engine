#include "Globals.h"

#include "GameObject.h"

#include "Component.h"

Component::Component(GameObject* owner, COMPONENT_TYPE type, const char* name, bool is_active) :
type(type),
owner(owner),
name(name),
is_active(is_active)
{

}

Component::~Component()
{

}

bool Component::Update()
{
	bool ret = true;

	return ret;
}

bool Component::CleanUp()
{
	bool ret = true;

	return ret;
}

// --- COMPONENT METHODS ---
const char* Component::GetName() const
{
	return name;
}

void Component::SetName(const char* new_name)
{
	name = new_name;
}
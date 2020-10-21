#include "E_Panel.h"

E_Panel::E_Panel(const char* name, bool is_active) : name(name), is_active(is_active)
{

}

E_Panel::~E_Panel()
{

}

bool E_Panel::Draw(ImGuiIO& io)
{
	bool ret = true;

	return ret;
}

bool E_Panel::CleanUp()
{
	bool ret = true;

	return ret;
}

// -------- E_PANEL METHODS --------
void E_Panel::Enable()
{
	if (!is_active)
	{
		is_active = true;
	}
	else
	{
		LOG("[EDITOR] %s Panel is already active!", name);
	}
}

void E_Panel::Disable()
{
	if (is_active)
	{
		is_active = false;
	}
	else
	{
		LOG("[EDITOR] %s Panel is already inactive!", name);
	}
}

bool E_Panel::IsActive() const
{
	return is_active;
}

const char* E_Panel::GetName() const
{
	return name;
}
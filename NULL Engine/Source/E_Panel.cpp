#include "E_Panel.h"

E_Panel::E_Panel(const char* name, bool is_active) : name(name), is_active(is_active), is_hovered(false)
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
}

void E_Panel::Disable()
{
	if (is_active)
	{
		is_active = false;
	}
}

bool E_Panel::IsActive() const
{
	return is_active;
}

bool E_Panel::IsHovered() const
{
	return is_hovered;
}

void E_Panel::SetIsHovered()
{	
	if (ImGui::IsWindowHovered())												// Checks if the last generated ImGui window is being hovered.
	{
		is_hovered = true;
	}
	else
	{
		is_hovered = false;
	}
}

const char* E_Panel::GetName() const
{
	return name;
}

void E_Panel::HelpMarker(const char* description)
{
	ImGui::TextDisabled("(?)");

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();

		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(description);
		ImGui::PopTextWrapPos();

		ImGui::EndTooltip();
	}
}
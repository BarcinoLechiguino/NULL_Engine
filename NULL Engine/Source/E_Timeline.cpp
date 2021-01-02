#include "VariableTypedefs.h"

#include "Time.h"

#include "E_Timeline.h"

E_Timeline::E_Timeline() : EditorPanel("Timeline")
{
	current_time = 0.0f;
}

E_Timeline::~E_Timeline()
{

}

bool E_Timeline::Draw(ImGuiIO& io)
{
	bool ret = true;

	static float values[2]	= { 1.0f, 15.0f };
	float values2[2]		= { 15.0f, 30.0f };
	static float values3[2] = { 5.0f, 25.0f };

	static float values4[6] = { 5.0f, 20.0f, 35.0f, 50.0f, 65.0f, 80.0f };

	ImGui::Begin("Timeline");
	static ImVec2 offset_and_scale = ImVec2(0.0f, 0.0f);
	ImGui::BeginTimelineEx("Current Animation", 120.0f, 12, 12, &offset_and_scale);
	//ImGui::PlotTimelineEvents("EVENTS", values4, true);
	ImGui::TimelineEventEx("Take001", values, true);
	ImGui::TimelineEventEx("Take002", values2, false);
	ImGui::TimelineEventEx("Transition", values3, false);
	ImGui::EndTimelineEx(12, current_time, ImGui::ColorConvertFloat4ToU32(ImVec4(0.8f, 0.25f, 0.25f, 1.0f)));

	ImGui::End();

	current_time += Time::Game::GetDT() * 24;																		// Time * Ticks Per Second

	if (current_time > 120.0f)
	{
		current_time = 0.0f;
	}

	return ret;
}

bool E_Timeline::CleanUp()
{
	bool ret = true;

	return ret;
}

// --- IMGUI TIMELINE METHODS ---

#ifndef __IMGUI_CUSTOM_H__
#define __IMGUI_CUSTOM_H__

#include "ImGui.h"

namespace ImGui
{
	// --- CURVE EDITOR ---



	// --------------------
	
	// --- THEME GENERATOR ---

	// From: { https://github.com/ocornut/imgui/issues/2265#issuecomment-465432091 }
	void ThemeGenerator		(bool is_window = false);
	
	inline ImVec4 MakeHigh	(float alpha);
	inline ImVec4 MakeMid	(float alpha);
	inline ImVec4 MakeLow	(float alpha);
	inline ImVec4 MakeBg	(float alpha, float offset = 0.0f);
	inline ImVec4 MakeText	(float alpha);

	// -----------------------

	// --- TIMELINE ---
	
	// From: { https://github.com/nem0/LumixEngine/blob/timeline_gui/external/imgui/imgui_user.inl#L814 }
	bool BeginTimeline		(const char* str_id, const float& max_value);
	bool TimelineEvent		(const char* str_id, float* values);
	void EndTimeline		();

	// From: { https://github.com/bakkesmodorg/PluginManager/blob/master/PluginManager/imgui/imguivariouscontrols.cpp#L3447 }
	bool BeginTimelineEx	(const char* str_id, const float& max_value, const int& num_visible_rows, const int& opt_exact_num_rows, ImVec2* popt_offset_and_scale);
	bool TimelineEventEx	(const char* str_id, float* values, bool keep_range_constant);
	void EndTimelineEx		(const int& num_vertical_grid_lines, const float& current_time, ImU32 timeline_running_color);

	// Own Modifications
	bool PlotTimelineEvents	(const char* str_id, float* values, bool keep_range_constant);
	
	// ----------------

	// --- WIDGET STATIC VARIABLES ---
	// --- CURVE EDITOR

	// ----------------
	// --- THEME GENERATOR
	static ImVec4		base			= ImVec4(0.502f, 0.075f, 0.256f, 1.0f);
	static ImVec4		bg				= ImVec4(0.200f, 0.220f, 0.270f, 1.0f);
	static ImVec4		text			= ImVec4(0.860f, 0.930f, 0.890f, 1.0f);
	static float		high_val		= 0.8f;
	static float		mid_val			= 0.5f;
	static float		low_val			= 0.3f;
	static float		window_offset	= -0.2f;
	// -------------------
	// --- TIMELINE
	static float		max_timeline_value;
	static const float	timeline_radius				= 6;

	static int			timeline_num_rows			= 0;
	static int			timeline_display_start		= 0;
	static int			timeline_display_end		= 0;
	static int			timeline_display_index		= 0;
	static ImVec2*		ptimeline_offset_and_scale	= NULL;
	// ------------
}

#endif // !__IMGUI_CUSTOM_H__
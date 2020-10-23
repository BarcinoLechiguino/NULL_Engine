#include "Application.h"
#include "M_Window.h"

#include "E_EngineConfiguration.h"

E_EngineConfiguration::E_EngineConfiguration() : E_Panel("Configuration")
{
	for (int i = 0; i < MAX_HISTOGRAM_SIZE; ++i)
	{
		FPS_data[i]	= 0;
		ms_data[i]	= 0;
	}
}

E_EngineConfiguration::~E_EngineConfiguration()
{

}

bool E_EngineConfiguration::Draw(ImGuiIO& io)
{
	bool ret = true;

	ImGui::Begin(GetName(), NULL, ImGuiWindowFlags_MenuBar);

	ImGui::BeginMenuBar();

	if (ImGui::BeginMenu("Options"))
	{
		if (ImGui::MenuItem("Default Settings"))
		{

		}

		if (ImGui::MenuItem("Load"))
		{

		}

		if (ImGui::MenuItem("Save"))
		{

		}
		
		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();

	ApplicationMenu();

	WindowMenu();
	RendererMenu();
	CameraMenu();

	InputMenu();

	FileSystemMenu();

	TexturesMenu();

	AudioMenu();

	PhysicsMenu();

	HardwareMenu();

	ImGui::End();

	return true;
}

bool E_EngineConfiguration::CleanUp()
{
	bool ret = true; 

	return true;
}

// ---------- ENGINE CONFIGURATION METHODS ----------
bool E_EngineConfiguration::ApplicationMenu()
{
	bool ret = true;

	if (ImGui::CollapsingHeader("Application"))
	{
		char buffer[128];
		strcpy_s(buffer, App->GetEngineName());

		if (ImGui::InputText("Engine Name", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			App->SetEngineName(buffer);
		}

		float average_FPS = 0.0f;
		float average_ms = 0.0f;

		for (int i = 0; i < MAX_HISTOGRAM_SIZE; ++i)
		{
			average_FPS += FPS_data[i];
			average_ms	+= ms_data[i];
		}

		average_FPS /= (float)MAX_HISTOGRAM_SIZE;
		average_ms	/= (float)MAX_HISTOGRAM_SIZE;

		char overlay[32];
		//sprintf(overlay, "Average FPS: %.2f", average_FPS);
		sprintf(overlay, "Framerate: %.2f", FPS_data[MAX_HISTOGRAM_SIZE - 1]);
		ImGui::PlotHistogram("FPS", FPS_data, IM_ARRAYSIZE(FPS_data), 0, overlay, 0.0f, 120.0f, ImVec2(0, 80));
		
		//sprintf(overlay, "Average ms: %.2f", average_ms);
		sprintf(overlay, "ms last frame: %.2f", ms_data[MAX_HISTOGRAM_SIZE - 1]);
		ImGui::PlotHistogram("MS", ms_data, IM_ARRAYSIZE(ms_data), 0, overlay, 0.0f, 40.0f, ImVec2(0, 80));
	}

	return ret;
}

bool E_EngineConfiguration::WindowMenu()
{
	bool ret = true;

	if (ImGui::CollapsingHeader("Window"))
	{

	}

	return ret;
}

bool E_EngineConfiguration::RendererMenu()
{
	bool ret = true;

	if (ImGui::CollapsingHeader("Renderer"))
	{

	}

	return ret;
}

bool E_EngineConfiguration::CameraMenu()
{
	bool ret = true;

	if (ImGui::CollapsingHeader("Camera"))
	{

	}

	return ret;
}

bool E_EngineConfiguration::FileSystemMenu()
{
	bool ret = true;

	if (ImGui::CollapsingHeader("File System"))
	{

	}

	return ret;
}

bool E_EngineConfiguration::InputMenu()
{
	bool ret = true;

	if (ImGui::CollapsingHeader("Input"))
	{

	}

	return ret;
}

bool E_EngineConfiguration::HardwareMenu()
{
	bool ret = true;

	if (ImGui::CollapsingHeader("Hardware"))
	{

	}

	return ret;
}

bool E_EngineConfiguration::TexturesMenu()
{
	bool ret = true;

	// NOTHING FOR NOW

	return ret;
}

bool E_EngineConfiguration::AudioMenu()
{
	bool ret = true;

	// NOTHING FOR NOW

	return ret;
}

bool E_EngineConfiguration::PhysicsMenu()
{
	bool ret = true;

	// NOTHING FOR NOW

	return ret;
}

void E_EngineConfiguration::UpdateFrameData(int frames, int ms)
{
	for (int i = 0; i < (MAX_HISTOGRAM_SIZE - 1); ++i)				// All elements in FPS[] and Ms[] are moved forward 1 position.
	{																// 
		FPS_data[i]	= FPS_data[i + 1];										// The last position is left "empty".
		ms_data[i]	= ms_data[i + 1];										// 
	}																// --------------------

	FPS_data[MAX_HISTOGRAM_SIZE - 1]	= frames;						// Adds to FPS[] the frames per second passed as argument to the last position in it.
	ms_data[MAX_HISTOGRAM_SIZE - 1]		= ms;							// Adds to Ms[] the ms per frame passed as argument to the last position in it.
}
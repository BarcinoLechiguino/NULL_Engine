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

	ConfigurationMenuBar();

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
bool E_EngineConfiguration::ConfigurationMenuBar()
{
	bool ret = true;
	
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

	return ret;
}

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

		PlotFrameDataHistogram();
		GenerateFrameCapSlider();
	}

	return ret;
}

bool E_EngineConfiguration::WindowMenu()
{
	bool ret = true;

	if (ImGui::CollapsingHeader("Window"))
	{
		// --- IS ACTIVE & ICON
		ImGui::Text("Is Active: ");		ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), App->window->IsActive() ? "True" : "False");
		ImGui::Text("Icon: ");			ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "None (WIP)");

		// --- WINDOW BRIGHTNESS
		float brightness = App->window->GetBrightness();
		ImGui::SliderFloat("Brightness", &brightness, 0.250f, 1.0f, "%.3f", NULL);
		App->window->SetBrightness(brightness);

		// --- WINDOW SIZE
		int width	= (int)App->window->GetWidth();
		int height	= (int)App->window->GetHeight();
		uint min_width, min_height, max_width, max_height;
		App->window->GetMinMaxSize(min_width, min_height, max_width, max_height);

		ImGui::SliderInt("Width", &width, min_width, max_width, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::SliderInt("Height", &height, min_height, max_height, "%d", ImGuiSliderFlags_AlwaysClamp);

		App->window->SetSize(width, height);

		// --- REFRESH RATE
		ImGui::Text("Refresh Rate: ");  ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", App->window->GetRefreshRate());

		// --- WINDOW MODE/FLAGS
		ImGui::Separator();
		ImGui::Text("Window Mode: ");

		bool fullscreen				= App->window->IsFullscreen();
		bool resizable				= App->window->IsResizable();
		bool borderless				= App->window->IsBorderless();
		bool fullscreen_desktop		= App->window->IsFullscreenDesktop();

		ImGui::Checkbox("Fullscreen", &fullscreen);
		{
			App->window->SetFullscreen(fullscreen);
		}

		ImGui::Checkbox("Resizable", &resizable);
		{
			App->window->SetResizable(resizable);
		}

		ImGui::Checkbox("Borderless", &borderless);
		{
			App->window->SetBorderless(borderless);
		}

		ImGui::Checkbox("Fullscreen Desktop", &fullscreen_desktop);
		{
			App->window->SetFullscreenDesktop(fullscreen_desktop);
		}
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

	if (ImGui::CollapsingHeader("Textures (WIP)"))
	{

	}

	return ret;
}

bool E_EngineConfiguration::AudioMenu()
{
	bool ret = true;

	if (ImGui::CollapsingHeader("Audio (WIP)"))
	{

	}

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

void E_EngineConfiguration::PlotFrameDataHistogram()
{
	float average_FPS	= 0.0f;
	float average_ms	= 0.0f;

	for (int i = 0; i < MAX_HISTOGRAM_SIZE; ++i)
	{
		average_FPS += FPS_data[i];
		average_ms	+= ms_data[i];
	}

	average_FPS /= (float)MAX_HISTOGRAM_SIZE;
	average_ms	/= (float)MAX_HISTOGRAM_SIZE;

	char overlay[32];
	sprintf(overlay, "Framerate: %.2f", FPS_data[MAX_HISTOGRAM_SIZE - 1]);
	ImGui::PlotHistogram("FPS", FPS_data, IM_ARRAYSIZE(FPS_data), 0, overlay, 0.0f, 120.0f, ImVec2(0, 80));
	ImGui::Text("Average FPS: ");	ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.2f", average_FPS);

	sprintf(overlay, "ms last frame: %.2f", ms_data[MAX_HISTOGRAM_SIZE - 1]);
	ImGui::PlotHistogram("MS", ms_data, IM_ARRAYSIZE(ms_data), 0, overlay, 0.0f, 40.0f, ImVec2(0, 80));
	ImGui::Text("Average ms: ");	ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.2f", average_ms);
}

void E_EngineConfiguration::GenerateFrameCapSlider()
{
	int cap = App->GetFrameCap();

	ImGui::SliderInt("Frame Cap", &cap, 0, 60, "%d", ImGuiSliderFlags_AlwaysClamp);

	App->SetFrameCap(cap);

	if (cap == 0)
	{
		App->frames_are_capped = false;
	}
	else
	{
		App->frames_are_capped = true;														// [ATTENTION] Could be troubling when trying to manage the framecap elsewhere.
	}
}
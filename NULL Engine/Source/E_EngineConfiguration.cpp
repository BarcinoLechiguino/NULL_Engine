#include "glew/include/glew.h"						// Maybe remove later so dependencies are kept to the minimum?

#include "Application.h"
#include "M_Window.h"
#include "M_Renderer3D.h"
#include "M_Camera3D.h"
#include "M_Input.h"
#include "M_FileSystem.h"
#include "HardwareInfo.h"

#include "E_EngineConfiguration.h"

#define MAX_INPUT_LOG_SIZE 1000

E_EngineConfiguration::E_EngineConfiguration() : E_Panel("Configuration")
{
	for (int i = 0; i < MAX_HISTOGRAM_SIZE; ++i)
	{
		FPS_data[i]	= 0;
		ms_data[i]	= 0;
	}

	input_log_scroll_to_bottom = true;
}

E_EngineConfiguration::~E_EngineConfiguration()
{

}

bool E_EngineConfiguration::Draw(ImGuiIO& io)
{
	bool ret = true;

	ImGui::Begin(GetName(), nullptr, ImGuiWindowFlags_MenuBar);

	SetIsHovered();
	
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

	ClearInputLog();

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
		ImGui::Text("Is Active");		ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), App->window->IsActive() ? "True" : "False");
		ImGui::Text("Icon:");			ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "None (WIP)");

		// --- WINDOW BRIGHTNESS
		GenerateBrightnessSlider();

		// --- WINDOW SIZE
		GenerateSizeSliders();

		// --- REFRESH RATE
		ImGui::Text("Refresh Rate:");  ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", App->window->GetRefreshRate());

		ImGui::Separator();

		// --- WINDOW MODE/FLAGS
		WindowModeFlags();
	}

	return ret;
}

bool E_EngineConfiguration::RendererMenu()
{
	bool ret = true;

	if (ImGui::CollapsingHeader("Renderer"))
	{
		// --- IS ACTIVE AND CURRENT DRIVER
		ImGui::Text("Is Active:");		ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), App->renderer->IsActive() ? "True" : "False");
		ImGui::Text("Driver:");			ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), App->renderer->GetDrivers());

		// --- VSYNC
		VsyncMode();

		ImGui::Separator();

		// --- RENDERER FLAGS
		RendererFlags();
	}

	return ret;
}

bool E_EngineConfiguration::CameraMenu()
{
	bool ret = true;

	if (ImGui::CollapsingHeader("Camera"))
	{
		// --- IS ACTIVE
		ImGui::Text("Is Active:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), App->camera->IsActive() ? "True" : "False");

		ImGui::SameLine(152.5f);

		if (ImGui::Button("Return to Origin"))
		{
			App->camera->ReturnToWorldOrigin();
		}

		// --- POSITION
		GenerateCameraPositionSlider();

		// --- REFERENCE
		GenerateCameraReferenceSlider();

		// --- MOVEMENT, ROTATION & ZOOM SPEED
		GenerateCameraSpeedSliders();
	}

	return ret;
}

bool E_EngineConfiguration::InputMenu()
{
	bool ret = true;

	if (ImGui::CollapsingHeader("Input"))
	{
		// --- IS ACTIVE
		ImGui::Text("Is Active:");		ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), App->input->IsActive() ? "True" : "False");
		
		// --- MOUSE POSITION, MOTION & WHEEL
		MouseInputData();

		ImGui::Separator();

		// --- INPUT LOG
		ImGui::BeginChild("Input Log");

		InputLogOutput();
		ReduceInputLog();
		InputLogScrollToBottom();

		ImGui::EndChild();
	}

	return ret;
}

bool E_EngineConfiguration::FileSystemMenu()
{
	bool ret = true;

	if (ImGui::CollapsingHeader("File System"))
	{
		ImGui::Text("Is Active:");		ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), App->file_system->IsActive() ? "True" : "False");

		GenerateBaseDirectoryText();
		GenerateReadDirectoriesText();
		GenerateWriteDirectoryText();
	}

	return ret;
}

bool E_EngineConfiguration::HardwareMenu()
{
	bool ret = true;

	if (ImGui::CollapsingHeader("System"))
	{
		HardwareInfo hw_info = App->GetHardwareInfo();

		if (ImGui::CollapsingHeader("Software"))
		{
			SDLInfo(&hw_info);

			ImGui::Separator();

			OpenGLInfo(&hw_info);

			ImGui::Separator();

			DevILInfo(&hw_info);
		}

		if (ImGui::CollapsingHeader("Hardware"))
		{
			CPUInfo(&hw_info);

			ImGui::Separator();

			GPUInfo(&hw_info);
		}
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
	for (uint i = 0; i < (MAX_HISTOGRAM_SIZE - 1); ++i)				// All elements in FPS[] and Ms[] are moved forward 1 position.
	{																// 
		FPS_data[i]	= FPS_data[i + 1];								// The last position is left "empty".
		ms_data[i]	= ms_data[i + 1];								// 
	}																// --------------------

	FPS_data[MAX_HISTOGRAM_SIZE - 1]	= (float)frames;			// Adds to FPS[] the frames per second passed as argument to the last position in it.
	ms_data[MAX_HISTOGRAM_SIZE - 1]		= (float)ms;				// Adds to Ms[] the ms per frame passed as argument to the last position in it.
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
	sprintf_s(overlay, "Framerate: %.2f", FPS_data[MAX_HISTOGRAM_SIZE - 1]);
	ImGui::PlotHistogram("FPS", FPS_data, IM_ARRAYSIZE(FPS_data), 0, overlay, 0.0f, 120.0f, ImVec2(0, 80));
	ImGui::Text("Average FPS: ");	ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.2f", average_FPS);

	sprintf_s(overlay, "ms last frame: %.2f", ms_data[MAX_HISTOGRAM_SIZE - 1]);
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

void E_EngineConfiguration::GenerateBrightnessSlider()
{
	float brightness = App->window->GetBrightness();
	ImGui::SliderFloat("Brightness", &brightness, 0.250f, 1.0f, "%.3f", 0);
	App->window->SetBrightness(brightness);
}

void E_EngineConfiguration::GenerateSizeSliders()
{
	int width = 0;
	int height = 0;

	SDL_GetWindowSize(App->window->GetWindow(), &width, &height);

	uint min_width, min_height, max_width, max_height = 0;
	App->window->GetMinMaxSize(min_width, min_height, max_width, max_height);

	ImGui::SliderInt("Width", &width, min_width, max_width, "%d", ImGuiSliderFlags_AlwaysClamp);
	ImGui::SliderInt("Height", &height, min_height, max_height, "%d", ImGuiSliderFlags_AlwaysClamp);

	App->window->SetSize(width, height);
}

void E_EngineConfiguration::WindowModeFlags()
{
	ImGui::Text("Window Mode:");

	bool maximized			= App->window->IsMaximized();
	bool fullscreen			= App->window->IsFullscreen();
	bool resizable			= App->window->IsResizable();
	bool borderless			= App->window->IsBorderless();
	bool fullscreen_desktop	= App->window->IsFullscreenDesktop();

	if (ImGui::Checkbox("Maximized", &maximized))
	{
		App->window->SetMaximized(maximized);
	}

	if (ImGui::Checkbox("Fullscreen", &fullscreen))
	{
		App->window->SetFullscreen(fullscreen);
	}

	if (ImGui::Checkbox("Resizable", &resizable))
	{
		App->window->SetResizable(resizable);
	}

	if (ImGui::Checkbox("Borderless", &borderless))
	{
		App->window->SetBorderless(borderless);
	}

	if (ImGui::Checkbox("Fullscreen Desktop", &fullscreen_desktop))
	{
		App->window->SetFullscreenDesktop(fullscreen_desktop);
	}
}

void E_EngineConfiguration::VsyncMode()
{
	bool vsync = App->renderer->GetVsync();
	if (ImGui::Checkbox("Vsync", &vsync))
	{
		App->renderer->SetVsync(vsync);
	}
}

void E_EngineConfiguration::RendererFlags()
{
	ImGui::Text("Renderer flags: ");
	
	// --- OPENGL FLAGS
	bool depth_test			= App->renderer->GetGLFlag(RENDERER_FLAGS::DEPTH_TEST);
	bool cull_face			= App->renderer->GetGLFlag(RENDERER_FLAGS::CULL_FACE);
	bool lighting			= App->renderer->GetGLFlag(RENDERER_FLAGS::LIGHTING);
	bool color_material		= App->renderer->GetGLFlag(RENDERER_FLAGS::COLOR_MATERIAL);
	bool texture_2D			= App->renderer->GetGLFlag(RENDERER_FLAGS::TEXTURE_2D);
	bool alpha_test			= App->renderer->GetGLFlag(RENDERER_FLAGS::ALPHA_TEST);
	bool blend				= App->renderer->GetGLFlag(RENDERER_FLAGS::BLEND);

	// --- SHOW FLAGS
	bool show_wireframe		= App->renderer->GetShowWireframe();

	// --- DRAW FLAGS
	bool draw_grid			= App->renderer->GetDrawWorldGrid();
	bool draw_axis			= App->renderer->GetDrawWorldAxis();


	// --- OPENGL FLAGS
	if (ImGui::Checkbox("Depth Test", &depth_test))
	{
		App->renderer->SetGLFlag(RENDERER_FLAGS::DEPTH_TEST, depth_test);
	}

	ImGui::SameLine(175.0f);

	if (ImGui::Checkbox("Cull Face", &cull_face))
	{
		App->renderer->SetGLFlag(RENDERER_FLAGS::CULL_FACE, cull_face);
	}

	if (ImGui::Checkbox("Lighting", &lighting))
	{
		App->renderer->SetGLFlag(RENDERER_FLAGS::LIGHTING, lighting);
	}

	ImGui::SameLine(175.0f);

	if (ImGui::Checkbox("Color Material", &color_material))
	{
		App->renderer->SetGLFlag(RENDERER_FLAGS::COLOR_MATERIAL, color_material);
	}

	if (ImGui::Checkbox("Texture 2D", &texture_2D))
	{
		App->renderer->SetGLFlag(RENDERER_FLAGS::TEXTURE_2D, texture_2D);
	}

	ImGui::SameLine(175.0f);

	if (ImGui::Checkbox("Alpha Test", &alpha_test))
	{
		App->renderer->SetGLFlag(RENDERER_FLAGS::ALPHA_TEST, alpha_test);
	}

	if (ImGui::Checkbox("Blend", &blend))
	{
		App->renderer->SetGLFlag(RENDERER_FLAGS::BLEND, blend);
	}

	ImGui::SameLine(175.0f);

	// --- SHOW FLAGS
	if (ImGui::Checkbox("Show Wireframes", &show_wireframe))
	{
		App->renderer->SetShowWireframe(show_wireframe);
	}

	// --- DRAW FLAGS
	if (ImGui::Checkbox("Draw World Grid", &draw_grid))
	{
		App->renderer->SetDrawWorldGrid(draw_grid);
	}

	ImGui::SameLine(175.0f);

	if (ImGui::Checkbox("Draw World Axis", &draw_axis))
	{
		App->renderer->SetDrawWorldAxis(draw_axis);
	}
}

void E_EngineConfiguration::GenerateCameraPositionSlider()
{
	/*float3 camera_position = App->camera->GetPosition();
	ImGui::DragFloat3("Position", (float*)&camera_position, 1.0f, 0.0f, 0.0f, "%.3f", NULL);
	App->camera->SetPosition(camera_position);*/

	vec3 camera_position = App->camera->GetPosition();
	ImGui::DragFloat3("Position", (float*)&camera_position, 1.0f, 0.0f, 0.0f, "%.3f", NULL);
	App->camera->SetPosition(camera_position);
}

void E_EngineConfiguration::GenerateCameraReferenceSlider()
{
	/*float3 camera_reference = App->camera->GetReference();
	ImGui::DragFloat3("Reference", (float*)&camera_reference, 1.0f, 0.0f, 0.0f, "%.3f", NULL);
	App->camera->SetReference(camera_reference);*/

	vec3 camera_reference = App->camera->GetReference();
	ImGui::DragFloat3("Reference", (float*)&camera_reference, 1.0f, 0.0f, 0.0f, "%.3f", NULL);
	App->camera->SetReference(camera_reference);
}

void E_EngineConfiguration::GenerateCameraSpeedSliders()
{
	float movement_speed = App->camera->GetMovementSpeed();
	float rotation_speed = App->camera->GetRotationSpeed();
	float zoom_speed = App->camera->GetZoomSpeed();

	ImGui::DragFloat("Movement Speed", &movement_speed, 0.01f, 0.0f, 0.0f, "%.3f", NULL);
	ImGui::DragFloat("Rotation Speed", &rotation_speed, 0.01f, 0.0f, 0.0f, "%.3f", NULL);
	ImGui::DragFloat("Zoom Speed", &zoom_speed, 0.01f, 0.0f, 0.0f, "%.3f", NULL);

	App->camera->SetMovementSpeed(movement_speed);
	App->camera->SetRotationSpeed(rotation_speed);
	App->camera->SetZoomSpeed(zoom_speed);
}

void E_EngineConfiguration::AddInputLog(const char* log)
{
	if (log != nullptr)
	{
		char* tmp = _strdup(log);
		
		input_logs.push_back(tmp);

		input_log_scroll_to_bottom = true;
	}
}

void E_EngineConfiguration::ReduceInputLog()
{
	if (input_logs.size() > MAX_INPUT_LOG_SIZE)
	{
		ClearInputLog();

		LOG("[WARNING] Cleared Input Log: Exceeded maximum input log size!");
	}
}

void E_EngineConfiguration::MouseInputData()
{
	ImGui::Text("Mouse Position:"); ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "x: %i y: %i", App->input->GetMouseX(), App->input->GetMouseY());
	ImGui::Text("Mouse Motion:");	ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "x: %i y: %i", App->input->GetMouseXMotionFromSDL(), App->input->GetMouseYMotionFromSDL());
	ImGui::Text("Mouse Wheel:");	ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "x: %i y: %i", App->input->GetMouseXWheel(), App->input->GetMouseYWheel());
}

void E_EngineConfiguration::InputLogOutput()
{
	for (uint i = 0; i < input_logs.size(); ++i)
	{
		ImVec4 text_colour = { 1.0f, 1.0f, 1.0f, 1.0f };									// White is the default colour, but added this to be able to easily change it.					

		if (strstr(input_logs[i], "[KEY]") != nullptr)
		{
			text_colour = { 0.0f, 1.0f, 1.0f, 1.0f };
		}
		
		if (strstr(input_logs[i], "[MOUSE]") != nullptr)
		{
			text_colour = { 1.0f, 0.0f, 1.0f, 1.0f };
		}

		ImGui::PushStyleColor(ImGuiCol_Text, text_colour);
		ImGui::TextUnformatted(input_logs[i]);
		ImGui::PopStyleColor();
	}
}

void E_EngineConfiguration::InputLogScrollToBottom()
{
	if (input_log_scroll_to_bottom)
	{
		ImGui::SetScrollHere(1.0f);

		input_log_scroll_to_bottom = false;
	}
}

void E_EngineConfiguration::ClearInputLog()
{
	for (uint i = 0; i < input_logs.size(); ++i)
	{
		free(input_logs[i]);
	}

	input_logs.clear();
}

void E_EngineConfiguration::GenerateBaseDirectoryText()
{
	ImGui::Text("Base Directory:");
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
	ImGui::TextWrapped(App->file_system->GetBaseDirectory());
	ImGui::PopStyleColor();
}

void E_EngineConfiguration::GenerateReadDirectoriesText()
{
	ImGui::Text("Read Directories:");
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
	ImGui::TextWrapped(App->file_system->GetReadDirectories());
	ImGui::PopStyleColor();
}

void E_EngineConfiguration::GenerateWriteDirectoryText()
{
	ImGui::Text("Write Directory:");
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
	ImGui::TextWrapped(App->file_system->GetWriteDirectory());
	ImGui::PopStyleColor();
}


void E_EngineConfiguration::SDLInfo(HardwareInfo* hw_info)
{
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "SDL Info:");

	ImGui::Text("SDL Version:");		ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", hw_info->SDL.sdl_version);
}

void E_EngineConfiguration::OpenGLInfo(HardwareInfo* hw_info)
{
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "OpenGL Info:");

	ImGui::Text("Model:");				ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", hw_info->OpenGL.model_name);
	ImGui::Text("Renderer:");			ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", hw_info->OpenGL.renderer_name);
	ImGui::Text("Version:");			ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", hw_info->OpenGL.version);
	ImGui::Text("Shading Language:");	ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", hw_info->OpenGL.shading_language_version);
}

void E_EngineConfiguration::DevILInfo(HardwareInfo* hw_info)
{
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "DevIL Info:");

	ImGui::Text("Vendor:");				ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", hw_info->DevIL.vendor);
	ImGui::Text("Version:");			ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", hw_info->DevIL.version);
}

void E_EngineConfiguration::CPUInfo(HardwareInfo* hw_info)
{
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "CPU Info:");

	ImGui::Text("CPUs:");				ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%u (Cache: %ukb)", hw_info->CPU.cpu_count, hw_info->CPU.cache_size);
	ImGui::Text("RAM Size:");			ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.1f GB", hw_info->CPU.ram_gb);

	ImGui::Text("Drivers:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s%s%s%s%s%s",
		hw_info->CPU.has_RDTSC		?	"RDTSC,"	: "",
		hw_info->CPU.has_AltiVec	?	"AltiVec,"	: "",
		hw_info->CPU.has_3DNow		?	"3DNow,"	: "",
		hw_info->CPU.has_MMX		?	"MMX,"		: "",
		hw_info->CPU.has_SSE		?	"SSE,"		: "",
		hw_info->CPU.has_SSE2		?	"SSE2,"		: "");
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s%s%s%s%s",
		hw_info->CPU.has_SSE3		?	"SSE3,"		: "",
		hw_info->CPU.has_SSE41		?	"SSE4.1,"	: "",
		hw_info->CPU.has_SSE42		?	"SSE4.2,"	: "",
		hw_info->CPU.has_RDTSC		?	"AVX,"		: "",
		hw_info->CPU.has_RDTSC		?	"AVX2,"		: "");
}

void E_EngineConfiguration::GPUInfo(HardwareInfo* hw_info)
{
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "GPU Info:");

	ImGui::Text("GPU:");				ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Vendor %i Device %i", hw_info->GPU.vendor, hw_info->GPU.device_id);
	ImGui::Text("Brand:");				ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", hw_info->GPU.brand);
	ImGui::Text("VRAM Budget:");		ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.1f MB", hw_info->GPU.vram_mb_budget);
	ImGui::Text("VRAM Usage:");			ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.1f MB", hw_info->GPU.vram_mb_usage);
	ImGui::Text("VRAM Available:");		ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.1f MB", hw_info->GPU.vram_mb_available);
	ImGui::Text("VRAM Reserved:");		ImGui::SameLine();	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.1f MB", hw_info->GPU.vram_mb_reserved);
}
#include <functional>			//function pointers
#include <algorithm>			//for_each()
#include <memory>				//Smart pointers

#include "Module.h"
#include "M_Window.h"
#include "M_Input.h"
#include "M_SceneIntro.h"
#include "M_Renderer3D.h"
#include "M_Editor.h"
#include "M_Camera3D.h"
#include "M_FileSystem.h"
#include "Configuration.h"

#include "Application.h"

Application::Application() :
quit(false),
debug(false), 
dt(0.0f),
hardware_info(),
window(nullptr),
input(nullptr),
scene_intro(nullptr),
editor(nullptr),
renderer(nullptr),
camera(nullptr),
file_system(nullptr)
{
	PERF_TIMER_START(perf_timer);
	
	// Modules -----------------------------------
	window			= new M_Window();
	input			= new M_Input();
	scene_intro		= new M_SceneIntro();
	editor			= new M_Editor();
	renderer		= new M_Renderer3D();
	camera			= new M_Camera3D();
	file_system		= new M_FileSystem();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(file_system);

	// Scenes
	AddModule(scene_intro);
	AddModule(editor);

	// Renderer last!
	AddModule(renderer);
	// -------------------------------------------

	// Save/Load variables
	want_to_load			= false;
	want_to_save			= false;
	user_has_saved			= false;

	// Framerate variables
	frame_cap				= 0;
	seconds_since_startup	= 0.0f;
	frames_are_capped		= FRAMES_ARE_CAPPED;
	frame_count				= 0;
	frames_last_second		= 0;
	prev_sec_frame_count	= 0;
	dt						= 0.0f;
	display_framerate_data	= false;

	pause					= false;

	PERF_TIMER_PEEK(perf_timer);
}

Application::~Application()
{
	std::vector<Module*>::reverse_iterator item = modules.rbegin();
	
	while (item != modules.rend())
	{
		//RELEASE((*item));
		
		delete (*item);
		++item;
	}
}

bool Application::Init()
{
	PERF_TIMER_START(perf_timer);
	
	LOG("Application Init --------------");

	bool ret = true;

	char* buffer = nullptr;

	uint size = file_system->Load("Configuration/configuration.JSON", &buffer);
	
	if (size <= 0)																	// Check if the configuration is empty and load the default configuration for the engine.
	{
		uint default_size = file_system->Load("Configuration/default_configuration.JSON", &buffer);

		if (default_size <= 0)
		{
			LOG("[error] Failed to load project settings.");
			return false;
		}
		
		engine_name			= TITLE;
		organization		= ORGANIZATION;
		frame_cap			= 60;
		frames_are_capped	= true;
	}

	engine_name = TITLE;														// Change Later?
	organization = ORGANIZATION;
	frame_cap = 60;
	frames_are_capped = true;

	Configuration config(buffer);
	Configuration node = config.GetNode("EditorState");

	std::vector<Module*>::iterator item = modules.begin();

	while (item != modules.end() && ret)
	{
		ret = (*item)->Init(node.GetNode((*item)->GetName()));		// Constructs and gives every module a handle for their own configuration node. M_Input -> "Input".
		++item;
	}
	
	// Initializing hardware info and Logging it.
	hardware_info.InitializeInfo();
	LogHardwareInfo();
	// -----------------------------------------

	LOG("Application Start --------------");

	item = modules.begin();

	while (item != modules.end() && ret)								// Move to start()?
	{
		if ((*item)->IsActive())
		{
			ret = (*item)->Start();
		}

		++item;
	}

	startup_timer.Start();

	PERF_TIMER_PEEK(perf_timer);

	//ms_timer.Start();
	return ret;
}

bool Application::Start()												// IS IT NEEDED?
{
	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");

	bool ret = true;

	std::vector<Module*>::iterator item = modules.begin();

	while (item != modules.end() && ret)
	{
		ret = (*item)->Start();
		++item;
	}
	
	return ret;
}

// Call PreUpdate, Update and PostUpdate on all modules
UPDATE_STATUS Application::Update()
{
	UPDATE_STATUS ret = UPDATE_STATUS::CONTINUE;
	
	if (quit)
	{
		return UPDATE_STATUS::STOP;
	}

	PrepareUpdate();

	if (ret == UPDATE_STATUS::CONTINUE)
	{
		ret = PreUpdate();
	}
	
	if (ret == UPDATE_STATUS::CONTINUE)
	{
		ret = DoUpdate();
	}

	if (ret == UPDATE_STATUS::CONTINUE)
	{
		ret = PostUpdate();
	}

	FinishUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	std::vector<Module*>::reverse_iterator item = modules.rbegin();

	while(item != modules.rend() && ret)
	{
		ret = (*item)->CleanUp();
		RELEASE((*item));

		++item;
	}

	modules.clear();

	hardware_info.CleanUp();

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	++frame_count;
	++frames_last_second;

	dt = frame_timer.ReadSec();
	frame_timer.Start();
}

UPDATE_STATUS Application::PreUpdate()
{
	UPDATE_STATUS ret = UPDATE_STATUS::CONTINUE;
	
	std::vector<Module*>::iterator item = modules.begin();

	while (item != modules.end() && ret == UPDATE_STATUS::CONTINUE)
	{
		if ((*item)->IsActive())
		{
			ret = (*item)->PreUpdate(dt);
		}

		++item;
	}

	if (ret == UPDATE_STATUS::THROW_ERROR)
	{
		LOG("PreUpdate threw an ERROR at Module %s.", (*item)->GetName());
	}

	return ret;
}

UPDATE_STATUS Application::DoUpdate()
{
	UPDATE_STATUS ret = UPDATE_STATUS::CONTINUE;

	std::vector<Module*>::iterator item = modules.begin();
	
	item = modules.begin();

	while (item != modules.end() && ret == UPDATE_STATUS::CONTINUE)
	{
		if ((*item)->IsActive())
		{
			if (!pause)
			{
				ret = (*item)->Update(dt);
			}
			else
			{
				ret = (*item)->Update(0.0f);
			}
		}

		++item;
	}

	if (ret == UPDATE_STATUS::THROW_ERROR)
	{
		LOG("Update threw an ERROR at Module %s.", (*item)->GetName());
	}

	return ret;
}

UPDATE_STATUS Application::PostUpdate()
{
	UPDATE_STATUS ret = UPDATE_STATUS::CONTINUE;

	std::vector<Module*>::iterator item = modules.begin();
	
	item = modules.begin();

	while (item != modules.end() && ret == UPDATE_STATUS::CONTINUE)
	{
		if ((*item)->IsActive())
		{
			ret = (*item)->PostUpdate(dt);
		}
		
		++item;
	}

	if (ret == UPDATE_STATUS::THROW_ERROR)
	{
		LOG("PostUpdate threw an ERROR at Module %s.", (*item)->GetName());
	}

	return ret;
}

void Application::FinishUpdate()
{
	if (want_to_load)
	{
		LoadConfigurationNow(load_config_file.c_str());

		want_to_load = false;
	}

	if (want_to_save)
	{
		SaveConfigurationNow(save_config_file.c_str());

		want_to_save = false;
	}

	// ------------ Framerate Calculations ------------
	// Frames in the last second.
	if (last_second_timer.ReadMs() > 1000)								// This here, initialize before this? Add another condition?
	{
		last_second_timer.Start();
		prev_sec_frame_count = frames_last_second;
		frames_last_second = 0;

		//LOG("%d frames last second", prev_sec_frame_count);
	}
	
	uint current_frame_ms = frame_timer.Read();

	// Frame cap and delay.
	if (frames_are_capped)
	{
		uint frame_cap_ms = 1000 / frame_cap;							// [ATTENTION] Quick fix to avoid cases where frame_cap = 0.

		if (current_frame_ms < frame_cap_ms)
		{
			precise_delay_timer.Start();

			uint required_delay = frame_cap_ms - current_frame_ms;

			SDL_Delay(required_delay);
		}
	}

	// Other frame calculations
	float avg_fps					= frame_count / startup_timer.ReadSec();
	uint32 ms_last_frame			= frame_timer.Read();
	uint32 frames_on_last_update	= prev_sec_frame_count;
	//seconds_since_startup			= startup_timer.ReadSec();
	startup_timer.AddTimeToClock();
	
	if (display_framerate_data)
	{
		static char framerate_data[256];

		sprintf_s(framerate_data, 256, "Av.FPS: %.2f / Last Frame Ms: %02u / Last sec frames: %i / Last dt: %.3f / Time since startup: %dh %dm %.3fs / Frame Count: %llu",
			avg_fps, ms_last_frame, frames_on_last_update, dt, startup_timer.clock.hours, startup_timer.clock.minutes, startup_timer.clock.seconds/*seconds_since_startup*/, frame_count);

		App->window->SetTitle(framerate_data);
	}
	else
	{
		App->window->SetTitle(engine_name.c_str());
	}

	// Editor: Configuration Frame Data Histograms
	UpdateFrameData(frames_on_last_update, ms_last_frame);
}
// ---------------------------------------------

void Application::AddModule(Module* module)
{
	modules.push_back(module);
}

float Application::GetDt() const
{
	if (!pause)
	{
		return dt;
	}
	else
	{
		return 0.0f;
	}
}

float Application::GetUnpausableDt() const
{
	return dt;
}

uint Application::GetFrameCap() const
{
	return frame_cap;
}

void Application::SetFrameCap(uint new_cap)
{
	frame_cap = new_cap;
}

const char* Application::GetEngineName() const
{
	return engine_name.c_str();
}

void Application::SetEngineName(const char* name)
{
	engine_name = name;

	App->window->SetTitle(engine_name.c_str());
}

const char* Application::GetOrganizationName() const
{
	return organization.c_str();
}

void Application::SetOrganizationName(const char* name)
{
	organization = name;
}

void Application::AddEditorLog(const char* log)
{
	if (!quit && editor != nullptr)													// Second condition is not really necessary. It's more of a reminder to keep it in mind.
	{
		std::string full_log = log;

		uint log_start = full_log.find_last_of("\\") + 1;							// Gets the position of the last "\" in the log string.
		uint log_end = full_log.size();												// The last position of the log will be equal to the size of it.

		std::string short_log = full_log.substr(log_start, log_end);				// Returns the string that is within the given positions.

		editor->AddConsoleLog(short_log.c_str());									// Priorized readability over reducing to AddConsoleLog(full_log.substr(log_start, log_end)).

		short_log.clear();
		full_log.clear();
	}
}

void Application::LogHardwareInfo()
{
	LOG(" ------------- CPU INFO ------------- ");
	LOG("CPU Cores: %d",						hardware_info.CPU.cpu_count);
	LOG("CPU Cache Size: %d ",					hardware_info.CPU.cache_size);
	LOG("CPU RAM Size: %.1f GB",				hardware_info.CPU.ram_gb);
	LOG(" --- DRIVERS --- ");
	LOG("CPU RDTSC: %s",						hardware_info.CPU.has_RDTSC		?	"True" : "False");
	LOG("CPU AltiVec: %s",						hardware_info.CPU.has_AltiVec	?	"True" : "False");
	LOG("CPU Now3D: %s",						hardware_info.CPU.has_3DNow		?	"True" : "False");
	LOG("CPU MMX: %s",							hardware_info.CPU.has_MMX		?	"True" : "False");
	LOG("CPU SSE: %s",							hardware_info.CPU.has_SSE		?	"True" : "False");
	LOG("CPU SSE2: %s",							hardware_info.CPU.has_SSE2		?	"True" : "False");
	LOG("CPU SSE3: %s",							hardware_info.CPU.has_SSE3		?	"True" : "False");
	LOG("CPU SSE4.1: %s",						hardware_info.CPU.has_SSE41		?	"True" : "False");
	LOG("CPU SSE4.2: %s",						hardware_info.CPU.has_SSE42		?	"True" : "False");
	LOG("CPU AVX: %s",							hardware_info.CPU.has_AVX		?	"True" : "False");
	LOG("CPU AVX2: %s",							hardware_info.CPU.has_AVX2		?	"True" : "False");

	LOG(" ------------- GPU INFO ------------- ");
	LOG("GPU Vendor %d Device %d",				hardware_info.GPU.vendor, hardware_info.GPU.device_id);
	LOG("GPU Brand: %s",						hardware_info.GPU.brand);
	LOG(" --- VRAM --- ");
	LOG("GPU VRAM Budget: %.1f MB",				hardware_info.GPU.vram_mb_budget);
	LOG("GPU VRAM Usage: %.1f MB",				hardware_info.GPU.vram_mb_usage);
	LOG("GPU VRAM Available: %.1f MB",			hardware_info.GPU.vram_mb_available);
	LOG("GPU VRAM Reserved: %.1f MB",			hardware_info.GPU.vram_mb_reserved);

	LOG(" ------------- SDL INFO ------------- ");
	LOG("SDL Version: %s",						hardware_info.SDL.sdl_version);

	LOG(" ------------- OPENGL INFO ------------- ");
	LOG("OpenGL Model: %s",						hardware_info.OpenGL.model_name);
	LOG("OpenGL Renderer: %s",					hardware_info.OpenGL.renderer_name);
	LOG("OpenGL Version: %s",					hardware_info.OpenGL.version);
	LOG("OpenGL Shading Language Version: %s",	hardware_info.OpenGL.shading_language_version);
	//LOG("OpenGL Extensions: %s",				hardware_info.OpenGL.extensions);

	/*for (int i = 0; i < hardware_info.OpenGL.extensions.size(); ++i)
	{
		LOG("OpenGL Extensions: %s",			hardware_info.OpenGL.extensions[i]);
	}*/
}

void Application::RequestBrowser(const char* link)
{
	ShellExecuteA(NULL, "open", link, NULL, "", 0);
}

void Application::UpdateFrameData(int frames, int ms)
{
	editor->UpdateFrameData(frames, ms);
}

HardwareInfo Application::GetHardwareInfo() const
{
	return hardware_info;
}

void Application::LoadConfiguration(const char* file)
{
	want_to_load = true;

	if (user_has_saved)
	{
		load_config_file = ("Configuration.json");
	}
	else
	{
		load_config_file = ("DefaultConfiguration.json");
	}
}

void Application::SaveConfiguration(const char* file)
{
	want_to_save = true;
	save_config_file = ("Configuration/configuration.json");
}

void Application::LoadConfigurationNow(const char* file)
{
	for (uint i = 0; i < modules.size(); ++i)
	{
		//JSON_Object* obj = 

		//modules[i]->LoadConfiguration(Configuration());
	}
}

void Application::SaveConfigurationNow(const char* file)
{
	Configuration config;
	Configuration node = config.SetNode("EditorState");
	
	for (uint i = 0; i < modules.size(); ++i)
	{
		modules[i]->SaveConfiguration(config.SetNode(modules[i]->GetName()));
	}

	char* buffer = nullptr;
	uint size = config.SerializeToBuffer(&buffer);

	file_system->Save(file, buffer, size);

	RELEASE_ARRAY(buffer);
}
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

Application::Application() : debug(false), renderPrimitives(true), dt(0.0f)
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
	frames_are_capped		= true;
	vsync_is_active			= true;
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

	Configuration config(buffer);
	Configuration node = config.GetNode("EditorState");

	std::vector<Module*>::iterator item = modules.begin();

	while (item != modules.end() && ret)
	{
		ret = (*item)->Init(node.GetNode((*item)->GetName()));		// Constructs and gives every module a handle for their own configuration node. M_Input -> "Input".
		++item;
	}
	
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
		++item;
	}
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
	
	// Frame cap and delay.
	uint frame_cap_ms		= 1000 / frame_cap;
	uint current_frame_ms	= frame_timer.Read();

	if (frames_are_capped)
	{
		if (current_frame_ms < frame_cap_ms)
		{
			precise_delay_timer.Start();

			uint required_delay = frame_cap_ms - current_frame_ms;

			SDL_Delay(required_delay);

			//LOG("Application waited for %d milliseconds and got back in %f", required_delay, precise_delay_timer.ReadMs());
		}
	}

	// Other frame calculations
	float avg_fps					= frame_count / startup_timer.ReadSec();
	uint32 last_frame_ms			= frame_timer.Read();
	uint32 frames_on_last_update	= prev_sec_frame_count;
	//seconds_since_startup			= startup_timer.ReadSec();
	startup_timer.AddTimeToClock();
	
	if (display_framerate_data)
	{
		static char framerate_data[256];

		sprintf_s(framerate_data, 256, "Av.FPS: %.2f / Last Frame Ms: %02u / Last sec frames: %i / Last dt: %.3f / Time since startup: %dh %dm %.3fs / Frame Count: %llu",
			avg_fps, last_frame_ms, frames_on_last_update, dt, startup_timer.clock.hours, startup_timer.clock.minutes, startup_timer.clock.seconds/*seconds_since_startup*/, frame_count);

		App->window->SetTitle(framerate_data);
	}
	else
	{
		App->window->SetTitle(engine_name.c_str());
	}
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

const char* Application::GetEngineName() const
{
	return engine_name.c_str();
}

const char* Application::GetOrganizationName() const
{
	return organization.c_str();
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
	for (int i = 0; i < modules.size(); ++i)
	{
		//JSON_Object* obj = 

		//modules[i]->LoadConfiguration(Configuration());
	}
}

void Application::SaveConfigurationNow(const char* file)
{
	Configuration config;
	Configuration node = config.SetNode("EditorState");
	
	for (int i = 0; i < modules.size(); ++i)
	{
		modules[i]->SaveConfiguration(config.SetNode(modules[i]->GetName()));
	}

	char* buffer = nullptr;
	uint size = config.SerializeToBuffer(&buffer);

	file_system->Save(file, buffer, size);

	RELEASE_ARRAY(buffer);
}

//for (Module* it : modules)
//{
//	it->Init();
//}

//std::for_each(modules.begin(), modules.end(),			//for_each() method. Inits all modules from begin to end.
//	[](Module* m) 
//{
//	m->Init();
//});

////Lambda [](){}
////[](Module* m) {ANumber += 1; m->Init(); };			//[this/&...]
//[this](Module* m) {this->CleanUp(); m->Init(); };		//[this/&...] what it is capturing (variables that it will store), () Arguments of the function, {} Function method.
//
//std::vector<Module*>::iterator it = modules.begin();	//Iterator call
//modules.rbegin;											//Last element.
//modules.begin;											//First element.
//
//auto ANumber = 1u;										//Auto is set to unsigned integer.
//auto BNumber = 1.f;										//Auto is set to float.
//auto CNumber = 1.0;										//Auto is set to double.
//
//while(it != modules.end() && ret == true)				//Iterating the modules in a vector.
//{
//	(*it)->Init();										//
//	it++;												//Iterator->next
//}

//std::function<bool(int, int)> FunctionPtr = AFunction;				//<function type/what it returns (arguments)> Name
//FunctionPtr(1, 2);													//Accesses AFunction.

//std::function<UPDATE_STATUS()> UpdatePtr = [this]() { return this->Update(); }; //Set as the pointer to the Update() function of Application.


//std::shared_ptr<Module> ModulePtr = std::make_shared<Module>();		//A share_pointer automatically deletes and keeps an object alive. Module will have a counter of how many shared_pointer points to it.
//std::weak_ptr<Module> WeakPtr = ModulePtr;							//Weak pointer allows to create a pointer towards an object that can be destroyed while this smart pointer points to it.

//ModulePtr2 = ModulePtr;												//Second smart_pointer.
#ifndef __APPLICATION_H__
#define __APPLICATION_H_

#include <vector>
#include <string>

#include "Globals.h"
#include "Timer.h"
#include "PerfectTimer.h"
#include "Configuration.h"
#include "HardwareInfo.h"

class Module;															// Modules are forward declared in call order.
class M_Window;
class M_Input;
class M_Scene;
class M_Renderer3D;
class M_Editor;
class M_Camera3D;
class M_FileSystem;

class Application
{
public:
	Application();
	~Application();

	bool			Init();
	bool			Start();
	UPDATE_STATUS	Update();
	bool			CleanUp();

public:
	void			AddModule(Module* module);

	float			GetDt() const;
	float			GetUnpausableDt() const;
	
	uint			GetFrameCap() const;
	void			SetFrameCap(uint frame);

	const char*		GetEngineName() const;
	void			SetEngineName(const char* engine_name);
	const char*		GetOrganizationName() const;
	void			SetOrganizationName(const char* organization_name);		

	void			EngineShortcuts();

	void			AddEditorLog(const char* log);
	void			LogHardwareInfo();
	void			RequestBrowser(const char* link);
	void			UpdateFrameData(int frames, int ms);

	HardwareInfo	GetHardwareInfo() const;

	void			LoadConfiguration(const char* file);
	void			SaveConfiguration(const char* file);

	//void InitializeModules();
	//void InitializeVariables();

private:
	void			PrepareUpdate();
	void			FinishUpdate();

	UPDATE_STATUS	PreUpdate();
	UPDATE_STATUS	DoUpdate();
	UPDATE_STATUS	PostUpdate();

	void			LoadConfigurationNow(const char* file);
	void			SaveConfigurationNow(const char* file);

public:
	M_Window*				window;
	M_Input*				input;
	M_Scene*				scene;
	M_Renderer3D*			renderer;
	M_Editor*				editor;
	M_Camera3D*				camera;
	M_FileSystem*			file_system;

	bool					pause;										// Will keep track of whether or not the applcation is currently paused.
	bool					quit;										// Will keep track of whether or not the application has to close.
	bool					debug;										// Will keep track of whether or not the application is in debug mode.

	// --- SAVE & LOAD
	bool					user_has_saved;								// Will keep track of whether or not the user has saved the application's state at least once since start-up.

	// --- FRAMERATE
	uint					frame_cap;									// Stores the cap value that will be aplied to the framerate.
	float					seconds_since_startup;						// Stores the total amount of seconds that have elapsed since application start.
	bool					frames_are_capped;							// Will keep track of whether or not the frames are currently capped.
	bool					display_framerate_data;						// Will keep track of whether or not to display the framerate data on the window's title.		

private:
	std::vector<Module*>	modules;									// Vector that will contain all the modules. Will be iterated to go through each state of each module.

	std::string				engine_name;								// Window title string. In this case the name of the Game Engine.
	std::string				organization;								// String that will store the name of the organization behind the development of this application.

	// --- LOAD & SAVE
	Configuration			config;										// Interface class that will be used to modify .json files with the parson library.
	
	bool					want_to_load;								// Will keep track of whether or not the user wants to load a configuration.
	bool					want_to_save;								// Will keep track of whether or not the user wants to save a configuration.
	std::string				load_config_file;							// Will store the name string of the configuration file to be loaded.
	std::string				save_config_file;							// Will store the name string og the configuration file to be loaded.

	// --- FRAMERATE
	uint64					frame_count;								// Will keep track of the amount of frames that have been processed since application start.
	Timer					startup_timer;								// Will keep track of the time that has elapsed since application start.
	Timer					frame_timer;								// Will keep track of the amount of milliseconds that have elapsed since last frame. dt comes from here.
	PerfectTimer			perf_timer;									// Will keep track of the time that different functions of Application take to process. High precision.
	PerfectTimer			last_second_timer;							// Will keep track of whether or not a second has elapsed since last FPS check.
	uint32					frames_last_second;							// Will keep track of the amount of frames that have been processed in the last second.
	uint32					prev_sec_frame_count;						// Will keep track of the frame count of the last second. Stores frames_last_second for an extra frame.

	PerfectTimer			precise_delay_timer;						// Will keep track of the amount of time spent delaying the processing of the next frame.
	float					dt;											// Equivalent to the amount of milliseconds that have elapsed in a frame. Keeps everything in the same timestep.

	// --- HARDWARE INFO
	HardwareInfo			hardware_info;								// All the info/data about the software and the hardware of the system will be extracted from here.
};

extern Application* App;												// Allows to access the Application module from anywhere in the project.

#endif // !__APPLICATION_H__
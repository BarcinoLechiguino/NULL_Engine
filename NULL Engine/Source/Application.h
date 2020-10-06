#ifndef __APPLICATION_H__
#define __APPLICATION_H_

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"

#include <vector>
#include <memory>

class Application
{
public:
	Application();
	~Application();

	bool Init();
	UPDATE_STATUS Update();
	bool CleanUp();

private:
	void PrepareUpdate();
	void FinishUpdate();

	UPDATE_STATUS PreUpdate();
	UPDATE_STATUS DoUpdate();
	UPDATE_STATUS PostUpdate();

	void AddModule(Module* module);

public:
	// Modules
	ModuleWindow*			window;
	ModuleInput*			input;
	ModuleSceneIntro*		scene_intro;
	ModuleRenderer3D*		renderer3D;
	ModuleCamera3D*			camera;

	bool					debug;
	bool					renderPrimitives;
private:

	Timer					ms_timer;
	float					dt;
	std::vector<Module*>	modules;
};

extern Application* App;						// Allows to access the Application module from anywhere in the project.

#endif // !__APPLICATION_H__
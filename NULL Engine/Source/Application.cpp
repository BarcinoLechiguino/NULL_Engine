#include "Application.h"

#include <functional>			//function pointers
#include <vector>				//Vector 
#include <algorithm>			//for_each()
#include <memory>				//Smart pointers

Application::Application() : debug(false), renderPrimitives(true), dt(0.16f)
{
	window			= new ModuleWindow();
	input			= new ModuleInput();
	scene_intro		= new ModuleSceneIntro();
	renderer3D		= new ModuleRenderer3D();
	camera			= new ModuleCamera3D();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	
	// Scenes
	AddModule(scene_intro);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	std::vector<Module*>::reverse_iterator item = modules.rbegin();
	
	while (item != modules.rend())
	{
		delete (*item);
		++item;
	}
}

bool AFunction(int a, int b) { return true; }

bool Application::Init()
{
	bool ret = true;

	App = this;

	// Call Init() in all modules
	std::vector<Module*>::iterator item = modules.begin();

	while(item != modules.end() && ret)
	{
		ret = (*item)->Init();
		++item;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	
	item = modules.begin();

	while(item != modules.end() && ret)
	{
		ret = (*item)->Start();
		++item;
	}
	
	ms_timer.Start();
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
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

UPDATE_STATUS Application::PreUpdate()
{
	UPDATE_STATUS ret = UPDATE_STATUS::CONTINUE;
	
	std::vector<Module*>::iterator item = modules.begin();

	while (item != modules.end() && ret == UPDATE_STATUS::CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
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
		ret = (*item)->Update(dt);
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
		ret = (*item)->PostUpdate(dt);
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

}
// ---------------------------------------------

void Application::AddModule(Module* module)
{
	modules.push_back(module);
}

Application* App = nullptr;

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
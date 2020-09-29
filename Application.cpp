#include "Application.h"

#include <functional>			//function pointers
#include <vector>				//Vector 
#include <algorithm>			//for_each()
#include <memory>				//Smart pointers

Application::Application() : debug(false), renderPrimitives(true), dt(0.16f)
{
	window = new ModuleWindow();
	input = new ModuleInput();
	audio = new ModuleAudio();
	scene_intro = new ModuleSceneIntro();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	
	// Scenes
	AddModule(scene_intro);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	p2List_item<Module*>* item = list_modules.getLast();
	
	while(item != NULL)
	{
		delete item->data;
		item = item->prev;
	}
}

bool AFunction(int a, int b) { return true; }

bool Application::Init()
{
	bool ret = true;

	App = this;

	// Call Init() in all modules
	p2List_item<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Init();
		item = item->next;
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

	//std::function<update_status()> UpdatePtr = [this]() { return this->Update(); }; //Set as the pointer to the Update() function of Application.


	//std::shared_ptr<Module> ModulePtr = std::make_shared<Module>();		//A share_pointer automatically deletes and keeps an object alive. Module will have a counter of how many shared_pointer points to it.
	//std::weak_ptr<Module> WeakPtr = ModulePtr;							//Weak pointer allows to create a pointer towards an object that can be destroyed while this smart pointer points to it.

	//ModulePtr2 = ModulePtr;												//Second smart_pointer.

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	p2List_item<Module*>* item = list_modules.getFirst();
	
	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PreUpdate(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->Update(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PostUpdate(dt);
		item = item->next;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
	//modules.push_back(mod);			//modules pushback to vector.
}

Application* App = nullptr;

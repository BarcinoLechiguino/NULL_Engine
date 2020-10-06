#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module("SceneManager", start_enabled)
{

}

ModuleSceneIntro::~ModuleSceneIntro()
{

}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void ModuleSceneIntro::HandleDebugInput()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		DebugSpawnPrimitive(new Sphere());
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		DebugSpawnPrimitive(new Cube());
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		DebugSpawnPrimitive(new Cylinder());
	}

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		for (uint n = 0; n < primitives.size(); n++)
		{
			primitives[n]->SetPos((float)(std::rand() % 40 - 20), 10.f, (float)(std::rand() % 40 - 20));
		}
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		//Get a vector indicating the direction from the camera viewpoint to the "mouse"
		float mouse_x_position = ((float)App->input->GetMouseX() / (float)App->window->Width()) * 2.f - 1.f;
		float mouse_y_position = -((float)App->input->GetMouseY() / (float)App->window->Height()) * 2.f + 1.f;

		const vec2 mousePos(mouse_x_position, mouse_y_position);
		
		const vec4 rayEye = inverse(App->renderer3D->ProjectionMatrix) * vec4(mousePos.x, mousePos.y, -1.f, 1.f);
		const vec4 rayWorld(inverse(App->camera->GetViewMatrix()) * vec4(rayEye.x, rayEye.y, -1.f, 0.f));

		vec3 Dir(rayWorld.x, rayWorld.y, rayWorld.z);
	}
}

void ModuleSceneIntro::DebugSpawnPrimitive(Primitive * p)
{
	primitives.push_back(p);
	p->SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
}

// Update
UPDATE_STATUS ModuleSceneIntro::Update(float dt)
{
	Plane p(vec3(0, 1, 0));
	p.axis = true;
	p.Render();

	if (App->debug == true)
	{
		HandleDebugInput();
	}

	for (uint n = 0; n < primitives.size(); n++)
	{
		primitives[n]->Update();
	}

	return UPDATE_STATUS::CONTINUE;
}

UPDATE_STATUS ModuleSceneIntro::PostUpdate(float dt)
{
	for (uint n = 0; n < primitives.size(); n++)
	{
		primitives[n]->Render();
	}

	return UPDATE_STATUS::CONTINUE;
}
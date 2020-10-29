#include "Globals.h"
#include "Application.h"
#include "M_Window.h"
#include "M_Camera3D.h"
#include "M_Renderer3D.h"
#include "M_Input.h"
#include "Primitive.h"

#include "M_SceneIntro.h"

M_SceneIntro::M_SceneIntro(bool is_active) : Module("SceneManager", is_active)
{

}

M_SceneIntro::~M_SceneIntro()
{

}

bool M_SceneIntro::Init(Configuration& config)
{
	return true;
}

// Load assets
bool M_SceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool M_SceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

bool M_SceneIntro::LoadConfiguration(Configuration& root)
{
	bool ret = true;

	return ret;
}

bool M_SceneIntro::SaveConfiguration(Configuration& root) const
{
	bool ret = true;

	return ret;
}

// -------------- SCENE METHODS --------------
void M_SceneIntro::HandleDebugInput()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_STATE::KEY_DOWN)
	{	
		DebugSpawnPrimitive(new Sphere(1.0f, 12, 24));
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_STATE::KEY_DOWN)
	{
		DebugSpawnPrimitive(new Cube());
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_STATE::KEY_DOWN)
	{
		DebugSpawnPrimitive(new Cylinder());
	}

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_STATE::KEY_DOWN)
	{
		for (uint n = 0; n < primitives.size(); n++)
		{
			primitives[n]->SetPos((float)(std::rand() % 40 - 20), 10.f, (float)(std::rand() % 40 - 20));
		}
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_STATE::KEY_DOWN)
	{
		//Get a vector indicating the direction from the camera viewpoint to the "mouse"
		float mouse_x_position = ((float)App->input->GetMouseX() / (float)App->window->GetWidth()) * 2.f - 1.f;
		float mouse_y_position = -((float)App->input->GetMouseY() / (float)App->window->GetHeight()) * 2.f + 1.f;

		const vec2 mousePos(mouse_x_position, mouse_y_position);
		
		const vec4 rayEye = inverse(App->renderer->ProjectionMatrix) * vec4(mousePos.x, mousePos.y, -1.f, 1.f);
		const vec4 rayWorld(inverse(App->camera->GetViewMatrix()) * vec4(rayEye.x, rayEye.y, -1.f, 0.f));

		vec3 Dir(rayWorld.x, rayWorld.y, rayWorld.z);
	}
}

void M_SceneIntro::DebugSpawnPrimitive(Primitive * p)
{
	primitives.push_back(p);
	p->SetPos(App->camera->position.x, App->camera->position.y, App->camera->position.z);
}

// Update
UPDATE_STATUS M_SceneIntro::Update(float dt)
{
	if (App->debug == true)
	{
		HandleDebugInput();
	}

	for (uint n = 0; n < primitives.size(); n++)
	{
		primitives[n]->Update();
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_STATE::KEY_DOWN)
	{	
		App->SaveConfiguration("Resources/Configuration/configuration.JSON");

		//DebugSpawnPrimitive(new Sphere(1.0f, 1.0f));
	}

	return UPDATE_STATUS::CONTINUE;
}

UPDATE_STATUS M_SceneIntro::PostUpdate(float dt)
{
	for (uint n = 0; n < primitives.size(); n++)
	{
		primitives[n]->Render();
	}

	return UPDATE_STATUS::CONTINUE;
}
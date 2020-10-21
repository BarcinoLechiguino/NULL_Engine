#include "Globals.h"
#include "Application.h"
#include "M_Input.h"

#include "M_Camera3D.h"

M_Camera3D::M_Camera3D(bool is_active) : Module("Camera3D", is_active)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

M_Camera3D::~M_Camera3D()
{}

// -----------------------------------------------------------------
bool M_Camera3D::Init(Configuration& root)
{
	//Position.x = root.GetNumber("X");
	//Position.y = root.GetNumber("Y");
	//Position.z = root.GetNumber("Z");
	
	return true;
}

// -----------------------------------------------------------------
bool M_Camera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool M_Camera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
bool M_Camera3D::LoadConfiguration(Configuration& root)
{
	bool ret = true;

	//Position.x = root.GetNumber("X");
	//Position.y = root.GetNumber("Y");
	//Position.z = root.GetNumber("Z");

	return ret;
}

bool M_Camera3D::SaveConfiguration(Configuration& root) const
{
	bool ret = true;

	//root.SetNumber("X", Position.x);
	//root.SetNumber("Y", Position.y);
	//root.SetNumber("Z", Position.z);

	LOG("SAVED CAMERA INFO");

	return ret;
}

// -----------------------------------------------------------------
UPDATE_STATUS M_Camera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	vec3 new_position(0,0,0);
	float speed = 3.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_STATE::KEY_REPEAT)
	{
		speed = 12.0f * dt;
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_STATE::KEY_REPEAT)
	{
		new_position.y += speed;
	}
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_STATE::KEY_REPEAT)
	{
		new_position.y -= speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_REPEAT)
	{
		new_position -= Z * speed;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_STATE::KEY_REPEAT)
	{
		new_position += Z * speed;
	}


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_STATE::KEY_REPEAT)
	{
		new_position -= X * speed;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_STATE::KEY_REPEAT)
	{
		new_position += X * speed;
	}

	Position += new_position;
	Reference += new_position;

	// Mouse motion ----------------

	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_STATE::KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if(Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);
	}

	return UPDATE_STATUS::CONTINUE;
}

// -----------------------------------------------------------------
void M_Camera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}
}

// -----------------------------------------------------------------
void M_Camera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);
}


// -----------------------------------------------------------------
void M_Camera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;
}

// -----------------------------------------------------------------
float* M_Camera3D::GetRawViewMatrix()
{
	CalculateViewMatrix();
	return &ViewMatrix;
}

mat4x4 M_Camera3D::GetViewMatrix()
{
	CalculateViewMatrix();
	return ViewMatrix;
}

// -----------------------------------------------------------------
void M_Camera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

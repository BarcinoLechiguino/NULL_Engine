#include "Globals.h"
#include "Application.h"
#include "M_Window.h"
#include "M_Input.h"

#include "M_Camera3D.h"

#define MOVEMENT_SPEED 6.0f
#define ROTATION_SPEED 0.25f
#define ZOOM_SPEED 24.0f

M_Camera3D::M_Camera3D(bool is_active) : Module("Camera3D", is_active)
{
	CalculateViewMatrix();

	X				= vec3(1.0f, 0.0f, 0.0f);
	Y				= vec3(0.0f, 1.0f, 0.0f);
	Z				= vec3(0.0f, 0.0f, 1.0f);

	position		= vec3(2.0f, 2.0f, 5.0f);
	reference		= vec3(0.0f, 0.0f, 0.0f);

	LookAt(reference);

	movement_speed	= MOVEMENT_SPEED;
	rotation_speed	= ROTATION_SPEED;
	zoom_speed		= ZOOM_SPEED;
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
	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_STATE::KEY_REPEAT)
	{
		WASDMovement();

		FreeLookAround();
	}

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_STATE::KEY_REPEAT)
	{	
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_STATE::KEY_REPEAT)
		{
			RotateAroundReference();
		}
	}

	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_STATE::KEY_REPEAT)
	{
		PanCamera();
	}

	if (App->input->GetMouseZ() != 0)
	{
		Zoom();
	}

	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_STATE::KEY_DOWN)
	{
		ReturnToWorldOrigin();
	}

	return UPDATE_STATUS::CONTINUE;
}

// -----------------------------------------------------------------
void M_Camera3D::PointAt(const vec3 &position, const vec3 &reference, bool RotateAroundReference)
{
	this->position = position;												// Updates the position to the given one.
	this->reference = reference;											// Updates the reference to the given one.

	Z = normalize(position - reference);									// Forward vector. Where the camera is looking.
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));						// The Right vector is reconstructed as Normal/Orthographic to Z. Cross product returns a vector perp to two others.
	Y = cross(Z, X);														// The Up vector is reconstructed as the cross product of Z and X. This means that Y is perpendicular to both.

	if(!RotateAroundReference)
	{
		this->reference = this->position;
		this->position += Z * 0.05f;
	}
}

// -----------------------------------------------------------------
void M_Camera3D::LookAt(const vec3 &spot)									// Almost identical to PointAt except only the reference and XYZ are updated. DOES NOT TRANSLATE.
{
	reference = spot;

	Z = normalize(position - reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);
}


// -----------------------------------------------------------------
void M_Camera3D::Move(const vec3 &Movement)
{
	position += Movement;
	reference += Movement;
}

// -----------------------------------------------------------------
void M_Camera3D::WASDMovement()
{
	vec3 new_position(0, 0, 0);
	float mov_speed = movement_speed * App->GetUnpausableDt();
	
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_STATE::KEY_REPEAT)
	{
		mov_speed = movement_speed * 2 * App->GetUnpausableDt();
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_STATE::KEY_REPEAT)
	{	
		new_position -= Z * mov_speed;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_STATE::KEY_REPEAT)
	{
		new_position += Z * mov_speed;
	}


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_STATE::KEY_REPEAT)
	{
		new_position -= X * mov_speed;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_STATE::KEY_REPEAT)
	{
		new_position += X * mov_speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_STATE::KEY_REPEAT)
	{
		new_position += Y * mov_speed;
	}
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_STATE::KEY_REPEAT)
	{
		new_position -= Y * mov_speed;
	}

	Move(new_position);
}

void M_Camera3D::FreeLookAround()
{
	// Free Look
	int dx = -App->input->GetMouseXMotion();							// Motion value registered by the mouse in the X axis. Negated so the camera behaves like it should.
	int dy = -App->input->GetMouseYMotion();							// Motion value registered by the mouse in the Y axis. Negated so the camera behaves like it should.

	float sensitivity = rotation_speed;									// Factor that will be applied to dx before constructing the angle with which to rotate the vectors.

	if (dx != 0)														// --- 
	{
		float delta_X = (float)dx * sensitivity;						// The value of the angle that we will rotate the camera by is very, very small, as it will be applied each frame.

		X = rotate(X, delta_X, vec3(0.0f, 1.0f, 0.0f));					// All vectors of the camera (X = Right, Y = Up, Z = Forward), will be rotated by the value of the angle (delta_X)
		Y = rotate(Y, delta_X, vec3(0.0f, 1.0f, 0.0f));					// The axis of rotation will be Y (yaw), not to confuse with the Y vector, which belongs to the camera.
		Z = rotate(Z, delta_X, vec3(0.0f, 1.0f, 0.0f));					// Keep in mind that X(Right) will always remain axis aligned.
	}

	if (dy != 0)														// Same as above but only affecting the Y and Z vectors, as X will act as the pivot of the rotation.
	{
		float delta_Y = (float)dy * sensitivity;

		Y = rotate(Y, delta_Y, X);										// As stated above, X(Right) will be used as the X axis (pitch) as, even if it is rotated, it will always be perfectly
		Z = rotate(Z, delta_Y, X);										// axis aligned in space, at least for this case.

		if (Y.y < 0.0f)													// If the y component of the Y(Up) vector is negative.
		{
			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);			// The y component of Z(Forward) will be recalculated.
			Y = cross(Z, X);											// A new Y(Up) vector orthogonal to both Z(Forward) and X(Right) (cross product) will be calculated.
		}
	}
}

void M_Camera3D::RotateAroundReference()								// Almost identical to FreeLookAround(), but instead of only modifying XYZ, the position of the camera is also modified.
{
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float sensitivity = rotation_speed;

	position -= reference;												// One of the two differences with FreeLookAround().

	if (dx != 0)
	{
		float delta_X = (float)dx * sensitivity;

		X = rotate(X, delta_X, vec3(0.0f, 1.0f, 0.0f));
		Y = rotate(Y, delta_X, vec3(0.0f, 1.0f, 0.0f));
		Z = rotate(Z, delta_X, vec3(0.0f, 1.0f, 0.0f));
	}

	if (dy != 0)
	{
		float delta_Y = (float)dy * sensitivity;

		Y = rotate(Y, delta_Y, X);
		Z = rotate(Z, delta_Y, X);

		if (Y.y < 0.0f)
		{
			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = cross(Z, X);
		}
	}

	position = reference + Z * length(position);						// While FreeLookAround() rotates the camera on its axis, this method also translates the camera.
}																		// This makes the camera rotate around the reference as the distance will be maintained but pos will be affected by Z.

void M_Camera3D::PanCamera()
{
	vec3 new_X(0, 0, 0);
	vec3 new_Y(0, 0, 0);

	int dx = App->input->GetMouseXMotion();
	int dy = App->input->GetMouseYMotion();

	if (dx != 0)
	{
		new_X = -dx * X * App->GetDt();
	}

	if (dy != 0)
	{
		new_Y = dy * Y * App->GetDt();
	}

	vec3 new_position = new_X + new_Y;

	Move(new_position);
}

void M_Camera3D::Zoom()
{	
	position -= Z * App->input->GetMouseZ() * zoom_speed * App->GetDt();						// The value is negated to make the zoom behave as it should (whl bck = bck / whl fwrd = fwrd).
}

void M_Camera3D::ReturnToWorldOrigin()
{
	PointAt(vec3(2.0f, 2.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f), false);
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
	vec3 T = { -dot(X, position), -dot(Y, position), -dot(Z, position) };			// The camera's current transform is calculated.
	
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, 
						X.y, Y.y, Z.y, 0.0f, 
						X.z, Y.z, Z.z, 0.0f, 
						T.x, T.y, T.z, 1.0f);

	ViewMatrixInverse = inverse(ViewMatrix);
}

vec3 M_Camera3D::GetPosition() const
{
	return position;
}

vec3 M_Camera3D::GetReference() const
{
	return reference;
}

vec3 M_Camera3D::GetSpot() const
{
	return vec3(0.0f, 0.0f, 0.0f);
}

void M_Camera3D::SetPosition(vec3 position)
{
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
}

void M_Camera3D::SetReference(vec3 reference)
{
	this->reference.x = reference.x;
	this->reference.y = reference.y;
	this->reference.z = reference.z;
}

void M_Camera3D::SetSpot(vec3 spot)
{

}

float M_Camera3D::GetMovementSpeed() const
{
	return movement_speed;
}

float M_Camera3D::GetRotationSpeed() const
{
	return rotation_speed;
}

float M_Camera3D::GetZoomSpeed() const
{
	return zoom_speed;
}

void M_Camera3D::SetMovementSpeed(float movement_speed)
{
	this->movement_speed = movement_speed;
}

void M_Camera3D::SetRotationSpeed(float rotation_speed)
{
	this->rotation_speed = rotation_speed;
}

void M_Camera3D::SetZoomSpeed(float zoom_speed)
{
	this->zoom_speed = zoom_speed;
}
#ifndef __M_INPUT_H__
#define __M_INPUT_H__

#include "Module.h"
#include "Globals.h"

#define MAX_MOUSE_BUTTONS 5

class Configuration;

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class M_Input : public Module
{
public:
	
	M_Input(bool is_active = true);
	~M_Input();

	bool Init(Configuration& config) override;
	UPDATE_STATUS PreUpdate(float dt) override;
	UPDATE_STATUS Update(float dt) override;
	bool CleanUp() override;

	KEY_STATE GetKey(int id) const
	{
		return keyboard[id];
	}

	KEY_STATE GetMouseButton(int id) const
	{
		return mouse_buttons[id];
	}

	int GetMouseX() const
	{
		return mouse_x;
	}

	int GetMouseY() const
	{
		return mouse_y;
	}

	int GetMouseZ() const
	{
		return mouse_z;
	}

	int GetMouseXMotion() const
	{
		return mouse_x_motion;
	}

	int GetMouseYMotion() const
	{
		return mouse_y_motion;
	}

private:
	KEY_STATE* keyboard;
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS];
	int mouse_x;
	int mouse_y;
	int mouse_z;
	int mouse_x_motion;
	int mouse_y_motion;
	//int mouse_z_motion;

	char* dropped_file_path = nullptr;
};

#endif // !__M_INPUT_H__
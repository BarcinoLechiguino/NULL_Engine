#ifndef __M_INPUT_H__
#define __M_INPUT_H__

#include "Module.h"
#include "Globals.h"

#define MAX_MOUSE_BUTTONS 5

class Configuration;

enum class KEY_STATE
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

	bool			Init(Configuration& config) override;
	UPDATE_STATUS	PreUpdate(float dt) override;
	UPDATE_STATUS	Update(float dt) override;
	bool			CleanUp() override;

	bool			LoadConfiguration(Configuration& root) override;
	bool			SaveConfiguration(Configuration& root) const override;

public:
	KEY_STATE		GetKey(int id) const;
	KEY_STATE		GetMouseButton(int id) const;
	uint			GetMaxNumScancodes() const;

	int				GetMouseX() const;
	int				GetMouseY() const;
	int				GetMouseZ() const;
	int				GetMouseXMotion() const;
	int				GetMouseYMotion() const;
	int				GetMouseXWheel() const;
	int				GetMouseYWheel() const;

	const char*		GetDroppedFilePath() const;

private:
	KEY_STATE* keyboard;
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS];
	uint max_num_scancodes;

	int mouse_x;
	int mouse_y;
	int mouse_z;
	int mouse_x_motion;
	int mouse_y_motion;
	//int mouse_z_motion;
	int mouse_x_wheel;
	int mouse_y_wheel;

	const char* dropped_file_path;
};

#endif // !__M_INPUT_H__
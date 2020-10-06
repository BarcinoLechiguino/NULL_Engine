// ----------------------------------------------------
// Module.h
// Interface for all engine modules.
// ----------------------------------------------------

#ifndef __MODULE_H__
#define __MODULE_H__

#include <vector>
#include <string>

#include "Globals.h"

#define MAX_MODULE_NAME_LENGTH 25

class Module
{
public:
	Module(const char* name, bool is_active = true);
	virtual ~Module();

	virtual bool Init();
	virtual bool Start();
	virtual UPDATE_STATUS PreUpdate(float dt);
	virtual UPDATE_STATUS Update(float dt);
	virtual UPDATE_STATUS PostUpdate(float dt);
	virtual bool CleanUp();

public:
	bool GetModuleState() const;							// Will return the current state of the module.
	bool SetModuleState(bool is_active);					// Will modify the state of the module. Will call Start() or CleanUp() depending on whether it has been activated or deactivated.

	const char* GetName() const;							// Will return the name of the module.

private:
	bool is_active;											// State in which the module is currently in. If it is active the module will be looped each frame.
	char name[MAX_MODULE_NAME_LENGTH];						// The module name string will be mainly used for debug/LOG purposes.
};

#endif //  !__MODULE_H__
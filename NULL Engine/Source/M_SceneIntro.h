#pragma once

#include "Module.h"
#include "Globals.h"

class Configuration;
class Primitive;

class M_SceneIntro : public Module
{
public:
	M_SceneIntro(bool is_active = true);
	~M_SceneIntro();

	bool Init(Configuration& config) override;
	bool Start() override;
	UPDATE_STATUS Update(float dt) override;
	UPDATE_STATUS PostUpdate(float dt) override;
	bool CleanUp() override;

private:
	void HandleDebugInput();
	void DebugSpawnPrimitive(Primitive* p);

	std::vector<Primitive*> primitives;
};

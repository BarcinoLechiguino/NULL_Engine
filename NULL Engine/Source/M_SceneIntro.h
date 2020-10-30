#pragma once

#include "Module.h"
#include "Globals.h"

class Configuration;
class Primitive;
class GameObject;

class M_SceneIntro : public Module
{
public:
	M_SceneIntro(bool is_active = true);
	~M_SceneIntro();

	bool			Init(Configuration& config) override;
	bool			Start() override;
	UPDATE_STATUS	Update(float dt) override;
	UPDATE_STATUS	PostUpdate(float dt) override;
	bool			CleanUp() override;

	bool LoadConfiguration(Configuration& root) override;
	bool SaveConfiguration(Configuration& root) const override;

public:
	GameObject* CreateGameObject(const char* name = nullptr, GameObject* parent = nullptr);

private:
	void HandleDebugInput();
	void DebugSpawnPrimitive(Primitive* p);

public:
	std::vector<GameObject*> game_objects;
	GameObject* root_object;

private:
	std::vector<Primitive*> primitives;
};

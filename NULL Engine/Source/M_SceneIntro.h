#pragma once

#include "Module.h"
#include "Globals.h"

class Configuration;
class Primitive;
class GameObject;
class R_Mesh;

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

	bool			LoadConfiguration(Configuration& root) override;
	bool			SaveConfiguration(Configuration& root) const override;

public:
	GameObject*		CreateGameObject(const char* name = nullptr, GameObject* parent = nullptr);
	bool			CreateGameObjectsFromModel(const char* path);
	bool			GenerateGameObjectsFromMeshes(const char* path, std::string file_name, std::vector<R_Mesh*>& meshes);
	bool			GenerateGameObjectComponents(const char* path, std::string file_name, GameObject* game_object, R_Mesh* mesh);			

	bool			ApplyNewTextureToSelectedGameObject(const char* path);

	void			DeleteGameObject(GameObject* game_object);
	
	bool			NameHasDuplicate(const char* name);
	void			AddDuplicateNumberToName(const char* name);

	void			ChangeSceneName(const char* new_name);

private:
	void HandleDebugInput();
	void DebugSpawnPrimitive(Primitive* p);

public:
	std::vector<GameObject*> game_objects;
	GameObject* root_object;

private:
	std::vector<Primitive*> primitives;
};

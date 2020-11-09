#pragma once

#include "Module.h"
#include "Globals.h"

class Configuration;
class Primitive;
class GameObject;
class R_Mesh;

class M_Scene : public Module
{
public:
	M_Scene(bool is_active = true);
	~M_Scene();

	bool			Init(Configuration& config) override;
	bool			Start() override;
	UPDATE_STATUS	Update(float dt) override;
	UPDATE_STATUS	PostUpdate(float dt) override;
	bool			CleanUp() override;

	bool			LoadConfiguration(Configuration& root) override;
	bool			SaveConfiguration(Configuration& root) const override;

public:
	GameObject*		CreateGameObject(const char* name = nullptr, GameObject* parent = nullptr);
	void			DeleteGameObject(GameObject* game_object);

	bool			CreateGameObjectsFromModel(const char* path);
	bool			GenerateGameObjectsFromMeshes(const char* path, std::string file_name, std::vector<R_Mesh*>& meshes);
	bool			GenerateGameObjectComponents(const char* path, std::string file_name, GameObject* game_object, R_Mesh* mesh);			

	bool			ApplyNewTextureToSelectedGameObject(const char* path);

	// --- ROOT GAME OBJECT METHODS ---
	GameObject*		GetRootGameObject() const;
	void			SetRootGameObject(GameObject* game_object);
	
	void			ChangeSceneName(const char* new_name);

	// --- SELECTED GAME OBJECT METHODS ---
	GameObject*		GetSelectedGameObject() const;
	void			SetSelectedGameObject(GameObject* game_object);

	void			DeleteSelectedGameObject();

	//bool			NameHasDuplicate(const char* name);
	//void			AddDuplicateNumberToName(const char* name);

private:
	void			HandleDebugInput();
	void			DebugSpawnPrimitive(Primitive* p);

public:

private:
	std::vector<GameObject*>	game_objects;
	GameObject*					root_object;
	GameObject*					selected_game_object;

	std::vector<Primitive*>		primitives;
};

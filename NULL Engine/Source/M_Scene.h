#ifndef __M_SCENE_H__
#define __M_SCENE_H__

#include "Module.h"

class Configuration;
class Primitive;
class GameObject;
class R_Mesh;

struct Transform;

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
	void			DeleteGameObject(GameObject* game_object, uint index = -1);

	bool			ImportScene(const char* path);
	bool			ApplyNewTextureToSelectedGameObject(const char* path);

	// --- ROOT GAME OBJECT METHODS ---
	GameObject*		GetRootGameObject() const;																//
	void			SetRootGameObject(GameObject* game_object);												//
	
	void			ChangeSceneName(const char* new_name);													//

	// --- SELECTED GAME OBJECT METHODS ---
	GameObject*		GetSelectedGameObject() const;															// 
	void			SetSelectedGameObject(GameObject* game_object);											// 

	void			DeleteSelectedGameObject();																// 

private:
	void			HandleDebugInput();
	void			DebugSpawnPrimitive(Primitive* p);

public:
	int a;

private:
	std::vector<GameObject*>	game_objects;
	GameObject*					root_object;
	GameObject*					selected_game_object;

	std::vector<Primitive*>		primitives;
};

#endif // !__M_SCENE_H__

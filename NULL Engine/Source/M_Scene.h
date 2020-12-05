#ifndef __M_SCENE_H__
#define __M_SCENE_H__

#include <map>

#include "Module.h"

class ParsonNode;
class Primitive;
class GameObject;
class R_Texture;

class M_Scene : public Module
{
public:
	M_Scene(bool is_active = true);
	~M_Scene();

	bool			Init				(ParsonNode& config) override;
	bool			Start				() override;
	UPDATE_STATUS	Update				(float dt) override;
	UPDATE_STATUS	PostUpdate			(float dt) override;
	bool			CleanUp				() override;

	bool			SaveConfiguration	(ParsonNode& root) const override;
	bool			LoadConfiguration	(ParsonNode& root) override;

public:
	bool			SaveScene								(ParsonNode& configuration) const;							// 
	bool			LoadScene								(ParsonNode& configuration);									// 

	std::vector<GameObject*>* GetGameObjects				();
	
	GameObject*		CreateGameObject						(const char* name = nullptr, GameObject* parent = nullptr);		// 
	void			DeleteGameObject						(GameObject* game_object, uint index = -1);						// 
																															// --- ROOT GAME OBJECT METHODS ---
	GameObject*		GetRootGameObject						() const;														//
	void			SetRootGameObject						(GameObject* game_object);										//
	void			ChangeSceneName							(const char* new_name);											//

																															// --- SELECTED GAME OBJECT METHODS ---
	void			DeleteSelectedGameObject				();																// 
	GameObject*		GetSelectedGameObject					() const;														// 
	void			SetSelectedGameObject					(GameObject* game_object);										// 
	bool			ApplyNewTextureToSelectedGameObject		(R_Texture* path);												//

private:
	void			HandleDebugInput();
	void			DebugSpawnPrimitive(Primitive* p);

private:
	std::vector<GameObject*>		game_objects;																			// 
	//std::map<uint32, GameObject*>	game_objects;																			//

	GameObject*					root_object;																				// 
	GameObject*					selected_game_object;																		// 

	std::vector<Primitive*>		primitives;
};

#endif // !__M_SCENE_H__

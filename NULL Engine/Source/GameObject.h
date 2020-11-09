#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <vector>
#include <map>
#include <string>

typedef unsigned int uint;

class Component;
class C_Transform;
class C_Mesh;
class C_Material;
class C_Light;
class C_Camera;
enum class COMPONENT_TYPE;

class GameObject
{
public:
	GameObject();
	GameObject(uint id, std::string name = "GameObject", bool is_active = true, bool is_static = false);
	~GameObject();

	bool			Update();
	bool			CleanUp();

public:
	void			FreeComponents();
	void			FreeChilds();

	bool			AddChild(GameObject* child);										// Adds the given child to the GO's childs vector. Also deletes it from the previous parent's childs.
	bool			NewParentIsOwnChild(GameObject* child);								// Rets. true if the passed child is being added as a child of one of it's children or ch of chs.
	bool			DeleteChild(GameObject* child);										// Deletes the given child from the childs vector. Returns false upon not finding the child.
	
	Component*		CreateComponent(COMPONENT_TYPE type);								// Creates a component of the given type and adds it to the components vector.
	Component*		GetComponent(COMPONENT_TYPE type);									// Iterates through the components vector and returns the component that matches the given type.
	const char*		GetComponentNameFromType(COMPONENT_TYPE type);						// Returns a string matching the given type. Mainly used for debug purposes (LOG).
	
	C_Transform*	GetTransformComponent();											// Returns the GameObject's Transform Component.	Returns nullptr on ERROR.
	C_Mesh*			GetMeshComponent();													// Returns the GameObject's Mesh Component.			Returns nullptr on ERROR.
	C_Material*		GetMaterialComponent();												// Returns the GameObject's Material Component.		Returns nullptr on ERROR.
	C_Light*		GetLightComponent();												// Returns the GameObject's Light Component.		Returns nullptr on ERROR.
	C_Camera*		GetCameraComponent();												// Returns the GameObject's Camera Component.		Returns nullptr on ERROR.

	void			SetID(uint id);

public:																					// --- GETTERS AND SETTERS
	const char* GetName() const;
	bool		IsActive() const;
	bool		IsStatic() const;

	void		SetName(const char* new_name);
	void		SetIsActive(const bool& set_to);
	void		SetIsStatic(const bool& set_to);

	void		SetChildsIsActive(const bool& set_to, GameObject* parent);
	void		SetChildsIsStatic(const bool& set_to, GameObject* parent);

public:
	std::vector<Component*>		components;
	std::vector<GameObject*>	childs;

	GameObject*					parent;

	C_Transform*				transform;												// Don't know what to do with this. Maybe keeping it like Unity? Or have it like the rest of components?

	bool						is_root_object;											// Will be set to true if this GameObject is M_Scene's root_object.

private:
	uint						id;
	std::string					name;

	bool						is_active;
	bool						is_static;

};

#endif // !__GAME_OBJECT_H__
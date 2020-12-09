#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <vector>
#include <string>

#include "MathGeoBoundingBox.h"

typedef unsigned __int32 uint32;

class ParsonNode;

class MeshRenderer;
class CuboidRenderer;

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
	GameObject(std::string name , bool is_active = true, bool is_static = false);
	~GameObject();

	bool			Update				();
	bool			CleanUp				();

	bool			SaveState			(ParsonNode& root) const;
	bool			LoadState			(ParsonNode& root);

public:
	void			FreeComponents						();
	void			FreeChilds							();
	
	void			GetRenderers						(std::vector<MeshRenderer>& mesh_renderers, std::vector<CuboidRenderer>& cuboid_renderers); // TODO: Get them elsewhere. Scene maybe?

	bool			SetParent							(GameObject* new_parent);

	bool			AddChild							(GameObject* child);					// Adds the given child to the GO's childs vector. Also deletes it from prev. parent's childs.
	bool			NewChildIsOwnParent					(GameObject* child);					// Rets. true if passed child is being added as a child of one of it's children or ch of chs.
	bool			DeleteChild							(GameObject* child);					// Deletes the given child from the childs vector. Returns false upon not finding the child.
	bool			HasChilds							() const;
	
	Component*		CreateComponent						(COMPONENT_TYPE type);					// Creates a component of the given type and adds it to the components vector.
	bool			DeleteComponent						(Component* component_to_delete);		// Deletes the given component from the Components vector. Returs False on ERROR.

public:																							// --- COMPONENT GETTERS AND SETTERS
	Component*		GetComponent						(COMPONENT_TYPE type);					// Iterates through the components vector and returns the comp. that matches the given type.
	const char*		GetComponentNameFromType			(COMPONENT_TYPE type);					// Returns a string matching the given type. Mainly used for debug purposes (LOG).
	
	C_Transform*	GetTransformComponent				();										// Returns the GameObject's Transform Component.	Returns nullptr on ERROR.
	C_Mesh*			GetMeshComponent					();										// Returns the GameObject's Mesh Component.			Returns nullptr on ERROR.
	C_Material*		GetMaterialComponent				();										// Returns the GameObject's Material Component.		Returns nullptr on ERROR.
	C_Light*		GetLightComponent					();										// Returns the GameObject's Light Component.		Returns nullptr on ERROR.
	C_Camera*		GetCameraComponent					();										// Returns the GameObject's Camera Component.		Returns nullptr on ERROR.

	std::vector<Component*> GetAllComponentsWithType	(COMPONENT_TYPE type);					// Returns a vector with all the comps. of the given type that the GameObject currently has.
	std::vector<C_Mesh*>	GetAllMeshComponents		();

public:																								// --- GAME OBJECT GETTERS AND SETTERS
	uint32			GetUID								() const;
	const char*		GetName								() const;									// 
	bool			IsActive							() const;									// 
	bool			IsStatic							() const;									// 

	void			ForceUID							(const uint32& UID);						// 
	void			SetName								(const char* new_name);						// 
	void			SetIsActive							(const bool& set_to);						// 
	void			SetIsStatic							(const bool& set_to);						// 
	void			SetChildsIsActive					(const bool& set_to, GameObject* parent);	// 
	void			SetChildsIsStatic					(const bool& set_to, GameObject* parent);	// 

	uint32			GetParentUID						() const;

public:
	std::vector<Component*>		components;
	std::vector<GameObject*>	childs;

	GameObject*					parent;
	C_Transform*				transform;													// Don't know what to do with this. Maybe like Unity? Or have it like the rest of comps?

	bool						is_master_root;												//
	bool						is_scene_root;												// Will be set to true if this GameObject is M_Scene's scene root object.
	bool						to_delete;													// Will determine whether or not the GameObject should be deleted. See M_Scene's DeleteGameObject().

	OBB							obb;
	AABB						aabb;

	float3*						obb_vertices;
	float3*						aabb_vertices;

	bool						show_bounding_boxes;

private:
	uint32						uid;
	std::string					name;
	bool						is_active;
	bool						is_static;

	uint32						parent_uid;													// Only for Serialization purposes. Maybe will be repurposed later.
};

#endif // !__GAME_OBJECT_H__
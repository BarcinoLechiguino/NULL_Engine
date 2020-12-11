#ifndef __COMPONENT_H__
#define __COMPONENT_H__

typedef unsigned __int32 uint32;

class ParsonNode;
class GameObject;

enum class COMPONENT_TYPE
{
	NONE,
	TRANSFORM,
	MESH,
	MATERIAL,
	LIGHT,
	CAMERA,
};

class Component
{
public:
	Component(GameObject* owner, COMPONENT_TYPE type, bool is_active = true);
	virtual ~Component();

	virtual bool Update		();
	virtual bool CleanUp	();

	virtual bool SaveState	(ParsonNode& root) const;
	virtual bool LoadState	(ParsonNode& root);

public:
	const char*		GetNameFromType		() const;										// Will return a string with the name of the component. Depends on COMPONENT_TYPE.
	
	uint32			GetID				() const;										// Will return the component's ID.
	void			ResetID				();												// Will reset the component's ID. WARNING: All references to this comp. will be lost (serialization).

	bool			IsActive			() const;										// 
	void			SetIsActive			(const bool& set_to);							// 

	COMPONENT_TYPE	GetType				() const;										//
	GameObject*		GetOwner			() const;										//

private:
	uint32			id;																	// 
	bool			is_active;															//

	COMPONENT_TYPE	type;																//
	GameObject*		owner;																// 
};

#endif // !__COMPONENT_H__
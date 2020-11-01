#include "Globals.h"
#include "Application.h"
#include "M_Renderer3D.h"

#include "Component.h"
#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"
#include "C_Light.h"

#include "GameObject.h"

GameObject::GameObject() :
id(0),
name("GameObject"),
is_active(true),
is_static(false),
parent(nullptr),
is_root_object(false)
{
	transform = (C_Transform*)CreateComponent(COMPONENT_TYPE::TRANSFORM);
}

GameObject::GameObject(uint id, std::string name, bool is_active, bool is_static) :
id(id),
name(name),
is_active(is_active),
is_static(is_static),
parent(nullptr),
is_root_object(false)
{
	if (name.empty())
	{
		name = "GameObject";
	}

	transform = (C_Transform*)CreateComponent(COMPONENT_TYPE::TRANSFORM);
	//TMP
	CreateComponent(COMPONENT_TYPE::MESH);
	CreateComponent(COMPONENT_TYPE::MATERIAL);
	CreateComponent(COMPONENT_TYPE::LIGHT);
}

GameObject::~GameObject()
{

}

bool GameObject::Update()
{
	bool ret = true;

	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->IsActive())
		{
			components[i]->Update();
		}
	}

	Render();

	return ret;
}

bool GameObject::CleanUp()
{
	bool ret = true;

	transform = nullptr;
	
	for (uint i = 0; i < components.size(); ++i)
	{
		components[i]->CleanUp();

		delete components[i];
		components[i] = nullptr;
	}

	components.clear();

	if (parent != nullptr)
	{
		parent->DeleteChild(this);
	}

	for (uint i = 0; i < childs.size(); ++i)
	{
		if (childs[i] != nullptr)
		{
			childs[i]->CleanUp();
			
			/*if (i < childs.size())
			{
				childs.erase(childs.begin() + i);
			}*/
			//delete childs[i];
			//childs[i] = nullptr;
		}
	}

	childs.clear();

	return ret;
}

// --- GAMEOBJECT METHODS ---
bool GameObject::AddChild(GameObject* child)
{
	bool ret = true;
	
	if (child->is_root_object)
	{
		LOG("[ERROR] Could not add child %s to %s: %s is current root object!", child->name.c_str(), name.c_str(), child->name.c_str());

		return false;
	}
	
	if (child->parent != nullptr)
	{
		bool deleted = child->parent->DeleteChild(child);

		if (!deleted)
		{
			//LOG("[WARNING] Could not find child %s in parent %s", child->name.c_str(), child->parent->name.c_str());
		}
	}

	C_Transform* child_transform = (C_Transform*)child->GetComponent(COMPONENT_TYPE::TRANSFORM);
	
	child_transform->recalculate_global_transform = true;

	child->parent = this;

	childs.push_back(child);

	return ret;
}

bool GameObject::DeleteChild(GameObject* child)
{
	bool ret = false;
	
	for (uint i = 0; i < childs.size(); ++i)
	{
		if (childs[i] == child)
		{
			childs.erase(childs.begin() + i);
			ret = true;
			break;
		}
	}
	
	return ret;
}

Component* GameObject::CreateComponent(COMPONENT_TYPE type)
{
	Component* component = nullptr;

	bool check_for_duplicates = false;

	switch(type)
	{
	case COMPONENT_TYPE::TRANSFORM:
		component = new C_Transform(this);
		check_for_duplicates = true;
		break;

	case COMPONENT_TYPE::MESH:
		component = new C_Mesh(this);
		break;

	case COMPONENT_TYPE::MATERIAL:
		component = new C_Material(this);
		check_for_duplicates = true;
		break;

	case COMPONENT_TYPE::LIGHT:
		component = new C_Light(this);
		break;
	}

	if (component != nullptr)
	{
		if (check_for_duplicates)
		{
			for (uint i = 0; i < components.size(); ++i)
			{
				if (type == components[i]->type)
				{
					LOG("[ERROR] %s Component could not be added to %s: No duplicates allowed!", component->GetName(), name.c_str());
					
					delete component;
					component = nullptr;

					return nullptr;
				}
			}
		}

		components.push_back(component);
	}

	return component;
}

Component* GameObject::GetComponent(COMPONENT_TYPE type)
{
	for (uint i = 0; i < components.size(); ++i)
	{
		if (components[i]->type == type)
		{
			return components[i];
		}
	}

	//LOG("[WARNING] Could not find %s Component in %s", GetComponentNameFromType(type), name.c_str());

	return nullptr;
}

const char* GameObject::GetComponentNameFromType(COMPONENT_TYPE type)
{
	switch (type)
	{
	case COMPONENT_TYPE::NONE:		{ return "None"; }		break;
	case COMPONENT_TYPE::TRANSFORM: { return "Transform"; } break;
	case COMPONENT_TYPE::MESH:		{ return "Mesh"; }		break;
	case COMPONENT_TYPE::MATERIAL:	{ return "Material"; }	break;
	case COMPONENT_TYPE::LIGHT:		{ return "Light"; }		break;
	case COMPONENT_TYPE::UNKNOWN:	{ return "Unknown"; }	break;
	}

	LOG("[ERROR] Could Not Get Component Name From Type");

	return "Invalid Component Type";
}

void GameObject::SetID(uint id)
{

}

void GameObject::Render()
{
	C_Transform* transform	= (C_Transform*)GetComponent(COMPONENT_TYPE::TRANSFORM);
	C_Mesh* mesh			= (C_Mesh*)GetComponent(COMPONENT_TYPE::MESH);
	C_Material* material	= (C_Material*)GetComponent(COMPONENT_TYPE::MATERIAL);

	uint tex_id = 0;

	if (transform != nullptr)
	{

	}
	
	if (material != nullptr)
	{
		if (material->IsActive())
		{
			if (material->GetMaterial() != nullptr)
			{
				tex_id = material->GetTextureId();
			}
		}
	}

	if (mesh != nullptr)
	{
		if (mesh->IsActive())
		{
			if (mesh->GetMesh() != nullptr)
			{
				App->renderer->DrawMesh(mesh->GetMesh(), tex_id);
			}
		}
	}
}

// --- GAME OBJECT GETTERS AND SETTERS ---
const char* GameObject::GetName() const
{
	return name.c_str();
}

bool GameObject::IsActive() const
{
	return is_active;
}

bool GameObject::IsStatic() const
{
	return is_static;
}

void GameObject::SetName(const char* new_name)
{
	name = new_name;
}

void GameObject::SetIsActive(const bool& set_to)
{
	is_active = set_to;

	SetChildsIsActive(set_to, this);
}

void GameObject::SetIsStatic(const bool& set_to)
{
	is_static = set_to;

	SetChildsIsStatic(set_to, this);
}

void GameObject::SetChildsIsActive(const bool& set_to, GameObject* parent)
{
	if (parent != nullptr)
	{
		for (uint i = 0; i < parent->childs.size(); ++i)
		{
			parent->childs[i]->is_active = set_to;

			SetChildsIsActive(set_to, parent->childs[i]);
		}
	}
}

void GameObject::SetChildsIsStatic(const bool& set_to, GameObject* parent)
{
	if (parent != nullptr)
	{
		for (uint i = 0; i < parent->childs.size(); ++i)
		{
			parent->childs[i]->is_static = set_to;

			SetChildsIsStatic(set_to, parent->childs[i]);
		}
	}
}
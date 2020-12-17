#include "VariableTypedefs.h"
#include "JSONParser.h"

#include "Time.h"

#include "Resource.h"
#include "R_Mesh.h"
#include "R_Material.h"
#include "R_Texture.h"
#include "R_Model.h"
#include "R_Folder.h"
#include "R_Scene.h"
#include "R_Animation.h"

#include "M_ResourceManager.h"

typedef std::pair<std::map<uint32, Resource*>::iterator, bool>	INSERT_RESULT;													// Also valid for emplace oparations.
typedef std::map<uint32, Resource*>::iterator					RESOURCE_ITEM;
typedef std::map<std::string, uint32>::iterator					FILE_ITEM;

M_ResourceManager::M_ResourceManager() : Module("ResourceManager"),
file_refresh_timer(0.0f),
file_refresh_rate(0.0f)
{

}

M_ResourceManager::~M_ResourceManager()
{

}
 
bool M_ResourceManager::Init(ParsonNode& configuration)
{
	bool ret = true;

	//file_refresh_rate = (float)configuration.GetNumber("RefreshRate");
	file_refresh_rate = 1.0f;

	return ret;
}

bool M_ResourceManager::Start()
{
	bool ret = true;

	return ret;
}

UPDATE_STATUS M_ResourceManager::PreUpdate(float dt)
{
	UPDATE_STATUS status = UPDATE_STATUS::CONTINUE;

	file_refresh_timer += Time::Real::GetDT();

	if (file_refresh_timer > file_refresh_rate)
	{
		// Refresh folders create .meta files for all those
		// files that do not have one.

		// Also update the .meta and reimport all those files
		// that have been modified.

		file_refresh_timer = 0.0f;
	}

	return status;
}

UPDATE_STATUS M_ResourceManager::Update(float dt)
{
	UPDATE_STATUS status = UPDATE_STATUS::CONTINUE;

	// Check for modifications on the mirror file in "Assets/"

	return status;
}

UPDATE_STATUS M_ResourceManager::PostUpdate(float dt)
{
	UPDATE_STATUS status = UPDATE_STATUS::CONTINUE;



	return status;
}

bool M_ResourceManager::CleanUp()
{
	bool ret = true;

	std::map<uint32, Resource*>::iterator r_item;
	for (r_item = resources.begin(); r_item != resources.end(); ++r_item)
	{
		r_item->second->CleanUp();
		RELEASE(r_item->second);
	}

	resources.clear();

	std::map<std::string, uint32>::iterator f_item;
	for (f_item = loaded_files.begin(); f_item != loaded_files.end(); ++f_item)
	{
		((std::string)f_item->first).clear();																// Does not look good at all, but if it works...
	}

	loaded_files.clear();

	return ret;
}

bool M_ResourceManager::SaveConfiguration(ParsonNode& configuration) const
{
	bool ret = true;



	return ret;
}

bool M_ResourceManager::LoadConfiguration(ParsonNode& configuration)
{
	bool ret = true;



	return ret;
}

// --- M_RESOURCEMANAGER METHODS ---
Resource* M_ResourceManager::CreateResource(RESOURCE_TYPE type)
{
	Resource* resource = nullptr;

	// Check for duplicates?

	switch (type)
	{
	case RESOURCE_TYPE::MESH:		{ resource = new R_Mesh(); }		break;
	case RESOURCE_TYPE::MATERIAL:	{ resource = new R_Material(); }	break;
	case RESOURCE_TYPE::TEXTURE:	{ resource = new R_Texture(); }		break;
	case RESOURCE_TYPE::MODEL:		{ resource = new R_Model(); }		break;
	case RESOURCE_TYPE::FOLDER:		{ resource = new R_Folder(); }		break;
	case RESOURCE_TYPE::SCENE:		{ resource = new R_Scene(); }		break;
	case RESOURCE_TYPE::ANIMATION:	{ resource = new R_Animation(); }	break;
	}

	if (resource != nullptr)
	{
		INSERT_RESULT result = resources.insert({ resource->GetUID(), resource });								// Inserting the newly created resource in the map along with it's UID.

		if (!result.second)																						// As keys are UIDs, it is highly unlikely for 2 elements to collide.
		{
			LOG("[ERROR] Created %s was already in std::map!", resource->GetTypeAsString());					// However, it never hurts to add a simple safety check such as this.
			resource->CleanUp();
			RELEASE(resource);
		}
	}

	return resource;
}

Resource* M_ResourceManager::AddResource(Resource* resource)
{
	if (resource == nullptr)
	{
		LOG("[ERROR] Resource Manager: Could not add the resource to the resources std::map! Error: Resource* was nullptr.");
		return nullptr;
	}
	
	if (IsResourceAlready(resource->GetAssetsFile()))
	{
		std::string assets_file = resource->GetAssetsFile();
		Resource* stored = GetResourceFromFile(assets_file);
		assets_file.clear();

		if (stored != nullptr)
		{
			LOG("[STATUS] Resource Manager: Assigning already existing resource!");
			
			resource->CleanUp();
			RELEASE(resource);

			return stored;
		}
		else
		{
			LOG("[ERROR] Resource Manager: resources and loaded_path std::maps are out of sync! Path: %s", resource->GetAssetsPath());
			loaded_files.erase(resource->GetAssetsFile());
		}
	}
	
	INSERT_RESULT result = resources.insert({ resource->GetUID(), resource });

	if (!result.second)
	{
		LOG("[ERROR] Resource Manager: Could not insert the resource in the resources std::map!");
		return nullptr;
	}
	else
	{
		LOG("[STATUS] Resource Manager: Successfully inserted %s in resources std::map!", resource->GetAssetsFile());
	}

	return resource;
}

bool M_ResourceManager::DeleteResource(uint32 UID)
{
	bool ret = true;

	if (resources.find(UID) != resources.end())
	{
		resources.erase(UID);
	}
	else
	{
		LOG("[ERROR] Resource Manager: Resource to delete was not inside the resources std::map!");
		ret = false;
	}

	return ret;
}

Resource* M_ResourceManager::GetResource(uint32 UID)
{
	//std::map<uint32, Resource*>::iterator item = resources.find(UID);

	RESOURCE_ITEM item = resources.find(UID);
	if (item != resources.end())
	{
		item->second->SetReferences(item->second->GetReferences() + 1);
		return item->second;
	}

	return nullptr;
}

std::map<uint32, Resource*> M_ResourceManager::GetResources() const
{
	return resources;
}

bool M_ResourceManager::IsResourceAlready(std::string assets_file)
{
	FILE_ITEM item = loaded_files.find(assets_file);

	return (item != loaded_files.end());
}

Resource* M_ResourceManager::GetResourceFromFile(std::string assets_file)
{
	if (IsResourceAlready(assets_file))
	{
		FILE_ITEM f_item		= loaded_files.find(assets_file);		
		RESOURCE_ITEM r_item	= resources.find(f_item->second);												// p_item->second == resource UID;

		if (r_item != resources.end())
		{
			return r_item->second;
		}
	}

	return nullptr;
}
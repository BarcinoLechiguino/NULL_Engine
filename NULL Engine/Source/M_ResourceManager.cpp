#include "VariableTypedefs.h"
#include "JSONParser.h"

#include "Time.h"

#include "Importer.h"
#include "I_Scenes.h"
#include "I_Meshes.h"
#include "I_Materials.h"
#include "I_Textures.h"
#include "I_Folders.h"
#include "I_Animations.h"

#include "Application.h"
#include "M_FileSystem.h"

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
file_refresh_timer	(0.0f),
file_refresh_rate	(0.0f)
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
// --- IMPORT FILE METHODS--
uint32 M_ResourceManager::ImportFile(const char* assets_path)
{
	uint32 resource_uid = 0;

	assets_path = GetValidAssetsPath(assets_path);

	if (assets_path == nullptr)
	{
		LOG("[ERROR] Resource Manager: Could not Import File from the given path! Error: Path was nullptr.");
		return 0;
	}

	// Get Metas, Check Metas, Choose to Import or Load.
	bool valid_metas = false;
	if (!valid_metas)
	{
		resource_uid = ImportFromAssets(assets_path);

		// Create/Update Metas.
	}
	else
	{
		// Get Library Path.
		//resource_uid = LoadFromLibrary(library_path);
	}

	return resource_uid;
}

uint32 M_ResourceManager::ImportFromAssets(const char* assets_path)
{
	uint32 resource_uid = 0;
	
	char* buffer = nullptr;
	uint read = App->file_system->Load(assets_path, &buffer);
	if (read > 0)
	{
		RESOURCE_TYPE type = GetTypeFromExtension(assets_path);
		Resource* resource = CreateResource(type, assets_path);

		bool success = false;
		switch (type)
		{
		case RESOURCE_TYPE::MODEL:		{success = Importer::ImportScene(buffer, read, (R_Model*)resource); }		break;
		case RESOURCE_TYPE::MESH:		{success = Importer::ImportMesh(buffer, (R_Mesh*)resource); }				break;
		case RESOURCE_TYPE::TEXTURE:	{success = Importer::ImportTexture(buffer, read, (R_Texture*)resource); }	break;
		}

		if (!success)
		{
			LOG("[ERROR] Resource Manager: Could not Import File! Check for [IMPORTER] errors in the Console Panel.");
			return 0;
		}

		SaveResourceToLibrary(resource);

		resource_uid = resource->GetUID();
	}

	return resource_uid;
}

uint32 M_ResourceManager::LoadFromLibrary(const char* library_path)
{
	return 0;
}

uint64 M_ResourceManager::SaveResourceToLibrary(Resource* resource)
{
	uint64 written = 0;
	
	char* buffer = nullptr;

	switch (resource->GetType())
	{
	case RESOURCE_TYPE::MODEL:		{ written = Importer::Scenes::Save((R_Model*)resource, &buffer); }			break;
	case RESOURCE_TYPE::MESH:		{ written = Importer::Meshes::Save((R_Mesh*)resource, &buffer); }			break;
	case RESOURCE_TYPE::MATERIAL:	{ written = Importer::Materials::Save((R_Material*)resource, &buffer); }	break;
	case RESOURCE_TYPE::TEXTURE:	{ written = Importer::Textures::Save((R_Texture*)resource, &buffer); }		break;
	case RESOURCE_TYPE::FOLDER:		{ written = Importer::Folders::Save((R_Folder*)resource, &buffer); }		break;
	case RESOURCE_TYPE::SCENE:		{ /*written = TODO: HAVE A FUNCTIONAL R_SCENE AND SAVE/LOAD METHODS*/ }		break;
	case RESOURCE_TYPE::ANIMATION:	{ written = Importer::Animations::Save((R_Animation*)resource, &buffer); }	break;
	}

	RELEASE_ARRAY(buffer);

	if (written == 0)
	{
		LOG("[ERROR] Resource Manager: Could not save Resource in the Library! Error: Check for Importer Errors in the Console Panel.");
		return 0;
	}

	SaveMetaFile(resource);

	return written;
}

bool M_ResourceManager::SaveMetaFile(Resource* resource) const
{
	bool ret = true;

	if (resource == nullptr)
	{
		LOG("[ERROR] Resource Manager: Could not Save the Meta File! Error: Given Resource* was nullptr.");
		return false;
	}

	ParsonNode meta_root = ParsonNode();
	meta_root.SetNumber("UID", resource->GetUID());																											// --- GENERAL RESOURCE META DATA
	meta_root.SetNumber("Type", (uint)resource->GetType());																									// 
	meta_root.SetString("Name", resource->GetAssetsFile());																									// 
	meta_root.SetString("LibraryPath", resource->GetLibraryPath());																							// 
	meta_root.SetNumber("ModificationTime", App->file_system->GetLastModTime(resource->GetAssetsPath()));													// -----------------------------

	resource->SaveMeta(meta_root);																															// --- RESOURCE-SPECIFIC META DATA

	char* buffer		= nullptr;
	std::string path	= resource->GetAssetsPath() + std::string(META_EXTENSION);
	uint64 written		= meta_root.SerializeToFile(path.c_str(), &buffer);																					// --- SERIALIZING TO META FILE
	if (written > 0)
	{
		LOG("[STATUS] Resource Manager: Successfully Saved the Meta File for Resource %lu! Path: %s", resource->GetUID(), path.c_str());
	}
	else
	{
		LOG("[ERROR] Resource Manager: Could not Save the Meta File of Resource %lu! Error: File System could not write the file.", resource->GetUID());
	}

	RELEASE_ARRAY(buffer);

	path.clear();

	return ret;
}

bool M_ResourceManager::LoadMetaFile(const char* meta_path)
{
	bool ret = true;

	return ret;
}

const char* M_ResourceManager::GetValidAssetsPath(const char* assets_path)
{
	if (assets_path == nullptr)
	{
		LOG("[ERROR] Resource Manager: Could not validate the Assets Path! Error: Given path was nullptr.");
		return nullptr;
	}
	
	std::string norm_path = App->file_system->NormalizePath(assets_path);

	uint dir_path_start = norm_path.find("Assets");
	if (dir_path_start != std::string::npos)
	{
		norm_path = norm_path.substr(dir_path_start, norm_path.size());
		assets_path = _strdup(norm_path.c_str());
	}
	else
	{
		LOG("[ERROR] Resource Manager: Could not validate the Assets Path! Error: Given path did not contain the Assets Directory");
		assets_path = nullptr;
	}

	norm_path.clear();

	return assets_path;
}

RESOURCE_TYPE M_ResourceManager::GetTypeFromExtension(const char* assets_path)
{
	RESOURCE_TYPE type = RESOURCE_TYPE::NONE;

	std::string extension = App->file_system->GetFileExtension(assets_path);

	if (extension == "fbx" || extension == "FBX" 
		|| extension == "obj" || extension == "OBJ")
	{
		type = RESOURCE_TYPE::MODEL;
	}
	else if (extension == "png" || extension == "PNG" 
			|| extension == "tga" || extension == "TGA" 
			|| extension == "dds" || extension == "DDS")
	{
		type = RESOURCE_TYPE::TEXTURE;
	}
	else if (extension == "json" || extension == "JSON")
	{
		type = RESOURCE_TYPE::SCENE;
	}
	else
	{
		LOG("[ERROR] Resource Manager: Could not import from the given Assets Path! Error: File extension is not supported!");
	}

	return type;
}

void M_ResourceManager::SetResourceAssetsPathAndFile(const char* assets_path, Resource* resource)
{
	resource->SetAssetsPath(assets_path);
	resource->SetAssetsFile(App->file_system->GetFileAndExtension(assets_path).c_str());
}

void M_ResourceManager::SetResourceLibraryPathAndFile(Resource* resource)
{
	resource->SetLibraryPathAndFile();
}

// --- RESOURCE METHODS ---
Resource* M_ResourceManager::CreateResource(RESOURCE_TYPE type, const char* assets_path)
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
			return nullptr;
		}

		if (assets_path != nullptr)
		{
			SetResourceAssetsPathAndFile(assets_path, resource);
		}

		SetResourceLibraryPathAndFile(resource);
	}

	return resource;
}

bool M_ResourceManager::AddResource(Resource* resource)
{
	if (resource == nullptr)
	{
		LOG("[ERROR] Resource Manager: Could not add the resource to the resources std::map! Error: Resource* was nullptr.");
		return false;
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
		return false;
	}
	else
	{
		LOG("[STATUS] Resource Manager: Successfully inserted %s in resources std::map!", resource->GetAssetsFile());
	}

	return true;
}

bool M_ResourceManager::DeleteResource(uint32 UID)
{
	bool ret = true;

	std::map<uint32, Resource*>::iterator item = resources.find(UID);
	if(item != resources.end())
	{
		item->second->CleanUp();
		RELEASE(item->second);
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

void M_ResourceManager::GetResources(std::map<uint32, Resource*>& resources) const
{
	resources = this->resources;
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
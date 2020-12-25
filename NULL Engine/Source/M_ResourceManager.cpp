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

#include "MemoryManager.h"

typedef std::pair<std::map<uint32, Resource*>::iterator, bool>	INSERT_RESULT;													// Also valid for emplace oparations.
typedef std::map<uint32, Resource*>::iterator					RESOURCE_ITEM;
typedef std::map<uint32, std::string>::iterator					LIBRARY_ITEM;
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

		// TMP Clear all Reference 0 resources
		RESOURCE_ITEM item;
		for (item = resources.begin(); item != resources.end(); ++item)
		{
			if (item->second->GetReferences() == 0)
			{
				uint32 resource_uid = item->second->GetUID();
				++item;																							// Dirty Fix to avoid crashes after Deallocating an element while iterating.

				DeallocateResource(resource_uid);
			}
		}

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
	bool meta_is_valid = MetaFileIsValid(assets_path);
	if (!meta_is_valid)
	{
		resource_uid = ImportFromAssets(assets_path); 

		if (resource_uid == 0)
		{
			LOG("[ERROR] Resource Manager: Could not Import File %s! Error: See [IMPORTER] and Resource Manager ERRORS.", assets_path);
			return 0;
		}
	}
	else
	{
		LOG("[WARNING] Resource Manager: Could not Import File to Library! Error: The file was already in the Library.");

		char* buffer			= nullptr;
		ParsonNode meta_root	= LoadMetaFile(assets_path, &buffer);
		if (!meta_root.NodeIsValid())
		{
			LOG("[ERROR] Resource Manager: Could not Load File from the given Assets Path! Error: Could not get the Meta Root Node.");
			return 0;
		}

		resource_uid = (uint32)meta_root.GetNumber("UID");

		RELEASE_ARRAY(buffer);
	}

	return resource_uid;
}

uint32 M_ResourceManager::ImportFromAssets(const char* assets_path)
{
	uint32 resource_uid = 0;
	
	if (assets_path == nullptr)
	{
		LOG("[ERROR] Resource Manager: Could not Import File from the given path! Error: Path was nullptr.");
		return 0;
	}

	char* buffer = nullptr;
	uint read = App->file_system->Load(assets_path, &buffer);
	if (read > 0)
	{
		RESOURCE_TYPE type = GetTypeFromAssetsExtension(assets_path);
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
			LOG("[ERROR] Resource Manager: Could not Import File %s! Error: Check for [IMPORTER] errors in the Console Panel.", assets_path);
			return 0;
		}

		SaveResourceToLibrary(resource);

		resource_uid = resource->GetUID();

		DeallocateResource(resource);

		RELEASE_ARRAY(buffer);
	}
	else
	{
		LOG("[ERROR] Resource Manager: Could not Import File %s! Error: File System could not Read the File.", assets_path);
		return 0;
	}

	return resource_uid;
}

uint32 M_ResourceManager::LoadFromLibrary(const char* assets_path)
{
	uint32 resource_uid = 0;
	
	std::string error_string = "[ERROR] Resource Manager: Could not Load File from the given Library Path";

	assets_path = GetValidAssetsPath(assets_path);

	if (assets_path == nullptr)
	{
		LOG("%s! Error: Path was nullptr.", error_string.c_str());
		return 0;
	}

	char* buffer = nullptr;
	ParsonNode meta_root	= LoadMetaFile(assets_path, &buffer);
	bool meta_is_valid		= MetaFileIsValid(meta_root);
	if (!meta_root.NodeIsValid())
	{
		LOG("%s! Error: Could not get the Meta Root Node.", error_string.c_str());
		return 0;
	}
	if (!meta_is_valid)
	{
		LOG("%s! Error: Could not Validate the Meta File.", error_string.c_str());
		RELEASE_ARRAY(buffer);
		return 0;
	}

	resource_uid					= (uint32)meta_root.GetNumber("UID");
	ParsonArray contained_array		= meta_root.GetArray("ContainedResources");
	
	if (resources.find(resource_uid) != resources.end())
	{
		RELEASE_ARRAY(buffer);
		return resource_uid;																									// If the File To Load's Resource is already in memory.
	}	

	Resource* result	= nullptr;
	result				= AllocateResource(resource_uid, assets_path);
	if (result == nullptr)
	{
		LOG("[ERROR] Resource Manager: Could not Allocate Resource %lu in memory!", resource_uid);
		RELEASE_ARRAY(buffer);
		return 0;
	}

	std::string contained_path = "";
	std::string contained_name = "";
	for (uint i = 0; i < contained_array.size; ++i)
	{
		ParsonNode contained_node = contained_array.GetNode(i);

		App->file_system->SplitFilePath(assets_path, &contained_path, nullptr, nullptr);									// --- TMP Until Something Functional Is In Place.
		contained_name	= contained_node.GetString("Name");																	// 
		contained_path += contained_name;																					// -----------------------------------------------

		uint32 contained_uid = (uint32)contained_node.GetNumber("UID");

		result = AllocateResource(contained_uid, contained_path.c_str());
		if (result == nullptr)
		{
			LOG("[WARNING] Resource Manager: Could not allocate Contained Resource! UID: %lu, Name: %s", contained_uid, contained_name.c_str());
		}

		contained_name.clear();
		contained_path.clear();
	}

	RELEASE_ARRAY(buffer);
	error_string.clear();

	return resource_uid;
}

uint M_ResourceManager::SaveResourceToLibrary(Resource* resource)
{	
	uint written = 0;
	
	if (resource == nullptr)
	{
		LOG("[ERROR] Resource Manager: Could not Save Resource to Library! Error: Resource* was nullptr.");
		return 0;
	}

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

	RELEASE_ARRAY(buffer);																												// TMP Commented. Breaks with MMGR

	if (written == 0)
	{
		LOG("[ERROR] Resource Manager: Could not save Resource in the Library! Error: Check for Importer Errors in the Console Panel.");
		return 0;
	}

	if (ResourceHasMetaType(resource))
	{
		SaveMetaFile(resource);
	}

	library.emplace(resource->GetUID(), resource->GetLibraryPath());

	return written;
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
		assets_path = _strdup(norm_path.c_str());																							// ATTENTION: Memory Leak?
	}
	else
	{
		LOG("[ERROR] Resource Manager: Could not validate the Assets Path! Error: Given path did not contain the Assets Directory");
		assets_path = nullptr;
	}

	norm_path.clear();

	return assets_path;
}

RESOURCE_TYPE M_ResourceManager::GetTypeFromAssetsExtension(const char* assets_path)
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

RESOURCE_TYPE M_ResourceManager::GetTypeFromLibraryExtension(const char* library_path)
{
	if (library_path == nullptr)
	{
		LOG("[ERROR] Resource Manager: Could not get the Resource Type from the Library File's Extension! Error: Given Library Path was nullptr.");
		return RESOURCE_TYPE::NONE;
	}
	
	RESOURCE_TYPE type		= RESOURCE_TYPE::NONE;
	std::string extension	= App->file_system->GetFileExtension(library_path);
	extension				= "." + extension;

	if (extension == MODELS_EXTENSION)
	{
		type = RESOURCE_TYPE::MODEL;
	}
	else if (extension == MESHES_EXTENSION)
	{
		type = RESOURCE_TYPE::MESH;
	}
	else if (extension == MATERIALS_EXTENSION)
	{
		type = RESOURCE_TYPE::MATERIAL;
	}
	else if (extension == TEXTURES_EXTENSION)
	{
		type = RESOURCE_TYPE::TEXTURE;
	}
	else if (extension == FOLDERS_EXTENSION)
	{
		type = RESOURCE_TYPE::FOLDER;
	}
	else if (extension == SCENES_EXTENSION)
	{
		type = RESOURCE_TYPE::SCENE;
	}
	else if (extension == ANIMATIONS_EXTENSION)
	{
		type = RESOURCE_TYPE::ANIMATION;
	}
	else
	{
		type = RESOURCE_TYPE::NONE;
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

// --- META FILE METHODS ---
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
	// ASSETS PATH?
	meta_root.SetString("LibraryPath", resource->GetLibraryPath());																							// 
	meta_root.SetNumber("ModificationTime", (double)App->file_system->GetLastModTime(resource->GetAssetsPath()));											// ------------------------------

	resource->SaveMeta(meta_root);																															// --- RESOURCE-SPECIFIC META DATA

	char* buffer		= nullptr;
	std::string path	= resource->GetAssetsPath() + std::string(META_EXTENSION);
	uint written		= meta_root.SerializeToFile(path.c_str(), &buffer);																					// --- SERIALIZING TO META FILE
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

/*ParsonNode M_ResourceManager::LoadMetaFile(const char* assets_path)
{
	if (assets_path == nullptr)
	{
		LOG("[ERROR] Resource Manager: Could not load the .meta File! Error: Given path was nullptr!");
	}

	std::string meta_path = assets_path + std::string(META_EXTENSION);

	char* buffer = nullptr;
	uint read = App->file_system->Load(meta_path.c_str(), &buffer);
	if (read == 0)
	{
		LOG("[ERROR] Resource Manager: Could not load the .meta File with Path: %s! Error: No Meta File exists with the given path.", meta_path);
	}

	meta_path.clear();

	return ParsonNode(buffer);
}*/

ParsonNode M_ResourceManager::LoadMetaFile(const char* assets_path, char** buffer)
{
	if (assets_path == nullptr)
	{
		LOG("[ERROR] Resource Manager: Could not load the .meta File! Error: Given path was nullptr!");
	}

	std::string meta_path	= assets_path + std::string(META_EXTENSION);
	uint read				= App->file_system->Load(meta_path.c_str(), buffer);
	if (read == 0)
	{
		LOG("[ERROR] Resource Manager: Could not load the .meta File with Path: %s! Error: No Meta File exists with the given path.", meta_path);
	}

	meta_path.clear();

	return ParsonNode(*buffer);
}

bool M_ResourceManager::MetaFileIsValid(const char* assets_path)
{
	bool ret = true;

	std::string meta_path		= assets_path + std::string(META_EXTENSION);													// Will be used for [ERROR] Logs.
	std::string error_string	= "[ERROR] Resource Manager: Could not validate Meta File " + meta_path;
	
	char* buffer = nullptr;
	ParsonNode meta_root = LoadMetaFile(assets_path, &buffer);
	if (!meta_root.NodeIsValid())
	{
		LOG("%s! Error: Could not get the Meta Root Node!", error_string.c_str());
		meta_path.clear();
		return false;
	}

	uint resource_uid				= (uint)meta_root.GetNumber("UID");
	ParsonArray contained_array		= meta_root.GetArray("ContainedResources");

	if (library.find(resource_uid) == library.end())
	{
		LOG("%s! Error: Resource UID could not be found in Library.", error_string.c_str());
		meta_path.clear();
		RELEASE_ARRAY(buffer);
		return false;
	}
	if (!contained_array.ArrayIsValid())
	{
		LOG("%s! Error: Could not get the ContainedResources Array from Meta Root!", error_string.c_str());
		meta_path.clear();
		RELEASE_ARRAY(buffer);
		return false;
	}

	for (uint i = 0; i < contained_array.size; ++i)
	{
		ParsonNode contained_node		= contained_array.GetNode(i);
		uint32 contained_resource_uid	= (uint32)contained_node.GetNumber("UID");

		if (library.find(contained_resource_uid) == library.end())
		{
			LOG("%s! Error: Contained Resource UID could not be found in Library.", error_string.c_str());
			meta_path.clear();
			RELEASE_ARRAY(buffer);
			return false;
		}
	}

	RELEASE_ARRAY(buffer);
	error_string.clear();
	meta_path.clear();

	return ret;
}

bool M_ResourceManager::MetaFileIsValid(ParsonNode& meta_root)
{
	bool ret = true;

	std::string error_string = "[ERROR] Resource Manager: Could not validate Meta File";

	if (!meta_root.NodeIsValid())
	{
		LOG("%s! Error: Could not get the Meta Root Node!", error_string.c_str());
		error_string.clear();
		return false;
	}

	uint resource_uid				= (uint)meta_root.GetNumber("UID");
	ParsonArray contained_array		= meta_root.GetArray("ContainedResources");

	if (library.find(resource_uid) == library.end())
	{
		LOG("%s! Error: Resource UID could not be found in Library.", error_string.c_str());
		error_string.clear();
		return false;
	}
	if (!contained_array.ArrayIsValid())
	{
		LOG("%s! Error: Could not get the ContainedResources Array from Meta Root!", error_string.c_str());
		error_string.clear();
		return false;
	}

	for (uint i = 0; i < contained_array.size; ++i)
	{
		ParsonNode contained_node		= contained_array.GetNode(i);
		if (!contained_node.NodeIsValid())
		{
			LOG("%s! Error: Could not parse node %u from the Contained Array.", error_string.c_str(), i);
			error_string.clear();
			return false;
		}

		uint32 contained_resource_uid	= (uint32)contained_node.GetNumber("UID");

		if (library.find(contained_resource_uid) == library.end())
		{
			LOG("%s! Error: Contained Resource UID could not be found in Library.", error_string.c_str());
			error_string.clear();
			return false;
		}
	}

	error_string.clear();

	return ret;
}

bool M_ResourceManager::ResourceHasMetaType(Resource* resource) const
{
	if (resource == nullptr)
	{
		LOG("[ERROR] Resource Manager: Could not check that Resource* had Meta Type! Error: Resource* was nullptr.");
		return false;
	}
	
	RESOURCE_TYPE type = resource->GetType();
	
	return (type == RESOURCE_TYPE::FOLDER
			|| type == RESOURCE_TYPE::MODEL
			|| type == RESOURCE_TYPE::TEXTURE);
}

// --- RESOURCE METHODS ---
Resource* M_ResourceManager::CreateResource(RESOURCE_TYPE type, const char* assets_path, const uint32& forced_UID)
{
	Resource* resource = nullptr;

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
		if (assets_path != nullptr)
		{
			SetResourceAssetsPathAndFile(assets_path, resource);
		}

		if (forced_UID != 0)
		{
			resource->ForceUID(forced_UID);
		}

		SetResourceLibraryPathAndFile(resource);
	}

	return resource;
}

bool M_ResourceManager::DeleteResource(const uint32& UID)
{
	bool ret = true;
	
	if (library.find(UID) != library.end())
	{
		library.erase(UID);
	}

	Resource* resource_to_delete = nullptr;

	std::map<uint32, Resource*>::iterator item = resources.find(UID);
	if(item == resources.end())
	{
		LOG("[ERROR] Resource Manager: Resource to delete was not inside the resources std::map!");
		ret = false;
	}

	resource_to_delete = item->second;
	if (resource_to_delete != nullptr)
	{
		resource_to_delete->CleanUp();
		RELEASE(resource_to_delete);
	}

	resources.erase(UID);

	return ret;
}

bool M_ResourceManager::DeleteResource(Resource* resource_to_delete)
{
	if (resource_to_delete == nullptr)
	{
		LOG("[ERROR] Resource Manager: Could not delete given Resource! Error: Given Resource* was nullptr.");
		return false;
	}
	
	uint32 resource_uid = resource_to_delete->GetUID();

	resource_to_delete->CleanUp();
	RELEASE(resource_to_delete);

	if (library.find(resource_uid) != library.end())
	{
		library.erase(resource_uid);
	}
	if (resources.find(resource_uid) != resources.end())
	{
		resources.erase(resource_uid);
	}

	return true;
}

Resource* M_ResourceManager::RequestResource(const uint32& UID)
{	
	if (UID == 0)
	{
		LOG("[ERROR] Resource Manager: Resource Request Failed! Error: Requested UID was 0.");
		return nullptr;
	}
	
	Resource* resource		= nullptr;
	RESOURCE_ITEM r_item	= resources.find(UID);
	
	if (r_item != resources.end())
	{
		r_item->second->SetReferences(r_item->second->GetReferences() + 1);
		resource = r_item->second;
	}

	return resource;
}

bool M_ResourceManager::FreeResource(const uint32& UID)
{
	bool ret = true;
	
	if (UID == 0)
	{
		LOG("[ERROR] Resource Manager: Free Resource Operation Failed! Error: Freed UID was 0.");
		return false;
	}
	
	RESOURCE_ITEM r_item = resources.find(UID);
	if (r_item != resources.end())
	{
		if (r_item->second->GetReferences() == 0)																					// Safety check in case resource already has 0 references.
		{
			DeallocateResource(UID);
		}
		
		r_item->second->SetReferences(r_item->second->GetReferences() - 1);

		if (r_item->second->GetReferences() == 0)																					// Deallocating the resource in case it has 0 references.
		{
			DeallocateResource(UID);
		}
	}
	else
	{
		LOG("[ERROR] Resource Manager: Free Resource Operation Failed! Error: Given UID could not be found in Resources Map.");
		return false;
	}

	return ret;
}

Resource* M_ResourceManager::AllocateResource(const uint32& UID, const char* assets_path)
{
	if (assets_path == nullptr)
	{
		LOG("[ERROR] Resource Manager: Could not allocate Resource in Memory! Error: Given Path was nullptr.");
		return nullptr;
	}

	std::string error_string = "[ERROR] Resource Manager: Could not allocate Resource {" + std::string(assets_path) + "} in Memory";

	if (UID == 0)
	{
		LOG("%s! Error: Invalid UID.", error_string.c_str());
		return nullptr;
	}
	if (library.find(UID) == library.end())
	{
		LOG("%s! Error: Resource could not be found in Library.", error_string.c_str());
		return nullptr;
	}

	char* buffer				= nullptr;
	const char* library_path	= library.find(UID)->second.c_str();
	uint read					= App->file_system->Load(library_path, &buffer);
	if (read == 0)
	{
		LOG("%s! Error: File system could not read File [%s]", error_string.c_str(), library_path);
		return nullptr;
	}

	RESOURCE_TYPE type	= GetTypeFromLibraryExtension(library_path);
	Resource* resource	= CreateResource(type, assets_path, UID);
	bool success		= false;
	switch (type)
	{
	case RESOURCE_TYPE::MODEL:		{ success = Importer::Scenes::Load(buffer, (R_Model*)resource); }				break;
	case RESOURCE_TYPE::MESH:		{ success = Importer::Meshes::Load(buffer, (R_Mesh*)resource); }				break;
	case RESOURCE_TYPE::MATERIAL:	{ success = Importer::Materials::Load(buffer, (R_Material*)resource); }			break;
	case RESOURCE_TYPE::TEXTURE:	{ success = Importer::Textures::Load(buffer, read, (R_Texture*)resource); }		break;
	case RESOURCE_TYPE::FOLDER:		{ success = Importer::Folders::Load(buffer, (R_Folder*)resource); }				break;
	case RESOURCE_TYPE::SCENE:		{ /*success = TODO: HAVE A FUNCTIONAL R_SCENE AND SAVE/LOAD METHODS*/ }			break;
	case RESOURCE_TYPE::ANIMATION:	{ success = Importer::Animations::Load(buffer, (R_Animation*)resource); }		break;
	}

	if (success)
	{
		resources.emplace(resource->GetUID(), resource);
		LOG("[STATUS] Resource Manager: Successfully Allocated Resource %s in Memory!", resource->GetAssetsFile());
	}
	else
	{
		DeleteResource(resource);																									// ATTENTION: This deletes from resources and library!.
		LOG("%s! Error: Importer could not load the Resource Data from File [%s].", error_string.c_str(), library_path);
	}

	RELEASE_ARRAY(buffer);
	error_string.clear();

	return resource;
}

bool M_ResourceManager::DeallocateResource(const uint32& UID)
{
	bool ret = true;
	
	if (UID == 0)
	{
		LOG("[ERROR] Resource Manager: Could not Deallocate Resource! Error: Given UID was 0");
		return false;
	}

	RESOURCE_ITEM item = resources.find(UID);
	if (item != resources.end())
	{
		item->second->CleanUp();
		RELEASE(item->second);
		resources.erase(UID);
		return true;
	}
	else
	{
		LOG("[ERROR] Resource Manager: Could not Deallocate Resource %lu! Error: Resource was not allocated in memory.", UID);
	}

	return ret;
}

bool M_ResourceManager::DeallocateResource(Resource* resource_to_deallocate)
{
	bool ret = true;

	if (resource_to_deallocate == nullptr)
	{
		LOG("[ERROR] Resource Manager: Could not deallocate Resource! Error: Given Resource* was nullptr");
		return false;
	}

	uint32 resource_uid = resource_to_deallocate->GetUID();

	resource_to_deallocate->CleanUp();
	RELEASE(resource_to_deallocate);

	if (resources.find(resource_uid) != resources.end())
	{
		resources.erase(resource_uid);
	}
	else
	{
		//LOG("[WARNING] Resource Manager: Deallocated Resource was not stored in the Resources Map!");
	}

	return ret;
}

void M_ResourceManager::GetResources(std::map<uint32, Resource*>& resources) const
{
	resources = this->resources;
}
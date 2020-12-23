#ifndef __M_RESOURCE_MANAGER_H__
#define __M_RESOURCE_MANAGER_H__

#include <map>
#include <string>

#include "Module.h"

class ParsonNode;
class Resource;

enum class RESOURCE_TYPE;

typedef unsigned __int32 uint32;

class M_ResourceManager : public Module
{
public:
	M_ResourceManager();
	~M_ResourceManager();

	bool			Init				(ParsonNode& configuration) override;
	bool			Start				() override;
	UPDATE_STATUS	PreUpdate			(float dt) override;
	UPDATE_STATUS	Update				(float dt) override;
	UPDATE_STATUS	PostUpdate			(float dt) override;
	bool			CleanUp				() override;

	bool			SaveConfiguration	(ParsonNode& configuration) const override;
	bool			LoadConfiguration	(ParsonNode& configuration) override;

public:																												// --- IMPORT FILE METHODS ---
	uint32			ImportFile						(const char* assets_path);										// 
	uint32			ImportFromAssets				(const char* assets_path);										// 
	uint			SaveResourceToLibrary			(Resource* resource);											// 
	
	uint32			LoadFromLibrary					(const char* assets_path);										// 

	const char*		GetValidAssetsPath				(const char* assets_path);
	RESOURCE_TYPE	GetTypeFromAssetsExtension		(const char* assets_path);
	RESOURCE_TYPE	GetTypeFromLibraryExtension		(const char* library_path);

	void			SetResourceAssetsPathAndFile	(const char* assets_path, Resource* resource);
	void			SetResourceLibraryPathAndFile	(Resource* resource);

public:																												// --- META FILE METHODS ---
	bool			SaveMetaFile					(Resource* resource) const;
	ParsonNode		LoadMetaFile					(const char* assets_path);
	
	bool			MetaFileIsValid					(const char* assets_path);
	bool			MetaFileIsValid					(ParsonNode& meta_root);
	bool			ResourceHasMetaType				(Resource* resource) const;

public:																												// --- RESOURCE METHODS ---
	Resource*		CreateResource					(RESOURCE_TYPE type, const char* assets_path = nullptr, const uint32& forced_UID = 0);	// 
	bool			DeleteResource					(const uint32& UID);																	//
	bool			DeleteResource					(Resource* resource_to_delete);															// FORCED DELETE
	void			GetResources					(std::map<uint32, Resource*>& resources) const;											// 
	
	Resource*		RequestResource					(const uint32& UID);																	// 
	bool			FreeResource					(const uint32& UID);																	// 
	
	Resource*		AllocateResource				(const uint32& UID, const char* assets_path = nullptr);									// 
	bool			DeallocateResource				(const uint32& UID);																	// 
	bool			DeallocateResource				(Resource* resource_to_deallocate);														// FORCED DEALLOCATE
	
private:
	std::map<uint32, Resource*>		resources;																		// Resources currently in memory.
	std::map<std::string, uint32>	loaded_files;																	// 
	std::map<uint32, std::string>	library;																		// UID and Library Path string of all loaded resources.

	float							file_refresh_timer;																// 
	float							file_refresh_rate;																// 
};

#endif // !__M_RESOURCE_MANAGER_H__
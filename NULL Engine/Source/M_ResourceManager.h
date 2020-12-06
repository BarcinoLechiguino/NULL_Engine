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

public:
	Resource*		CreateResource		(RESOURCE_TYPE type);
	Resource*		AddResource			(Resource* resource);
	bool			DeleteResource		(uint32 UID);
	Resource*		GetResource			(uint32 UID);

	const char*		AddLoadedPath		(std::string assets_file);
	const char*		DeleteLoadedPath	(std::string assets_file);
	bool			IsResourceAlready	(std::string assets_file);
	Resource*		GetResourceFromFile	(std::string assets_file);

private:
	std::map<uint32, Resource*>		resources;
	std::map<std::string, uint32>	loaded_files;
};

#endif // !__M_RESOURCE_MANAGER_H__
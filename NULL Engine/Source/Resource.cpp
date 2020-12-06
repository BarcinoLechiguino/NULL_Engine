#include "VariableTypedefs.h"
#include "FileSystemDefinitions.h"

#include "Random.h"

#include "Resource.h"

Resource::Resource(RESOURCE_TYPE type) : 
type			(type),
id				(Random::LCG::GetRandomUint()),
name			("[NONE]"),
assets_path		("[NONE]"), 
assets_file		("[NONE]"), 
library_path	("[NONE]"), 
library_file	("[NONE]")
{

}

Resource::~Resource()
{

}

bool Resource::CleanUp()
{
	bool ret = true;

	assets_path.clear();
	assets_file.clear();
	library_path.clear();
	library_file.clear();

	return ret;
}

// --- RESOURCE METHODS ---
RESOURCE_TYPE Resource::GetType() const
{
	return type;
}

const char* Resource::GetTypeAsString() const
{
	switch (type)
	{
	case RESOURCE_TYPE::MESH:		{ return "MESH"; }		break;
	case RESOURCE_TYPE::MATERIAL:	{ return "MATERIAL"; }	break;
	case RESOURCE_TYPE::TEXTURE:	{ return "TEXTURE"; }	break;
	}

	return "NONE";
}

uint32 Resource::GetID() const
{
	return id;
}

void Resource::ResetID()
{
	id = Random::LCG::GetRandomUint();
}

const char* Resource::GetAssetsPath() const
{
	return assets_path.c_str();
}
const char* Resource::GetAssetsFile() const
{
	return assets_file.c_str();
}
const char* Resource::GetLibraryPath() const
{
	return library_path.c_str();
}
const char* Resource::GetLibraryFile() const
{
	return library_file.c_str();
}

void Resource::SetAssetsPath(const char* assets_path)
{
	this->assets_path = assets_path;
}

void Resource::SetAssetsFile(const char* assets_file)
{
	this->assets_file = assets_file;
}

void Resource::SetLibraryPath(const char* library_path)
{
	this->library_path = library_path;
}

void Resource::SetLibraryFile(const char* library_file)
{
	this->library_file = library_file;
}

void Resource::SetLibraryPathAndFile()
{
	std::string directory	= "";
	std::string file		= std::to_string(id);
	std::string extension	= "";
	
	switch (type)
	{
	case RESOURCE_TYPE::MESH:
		directory = MESHES_PATH;
		extension = MESH_EXTENSION;
		break;

	case RESOURCE_TYPE::MATERIAL:
		directory = MATERIALS_PATH;
		extension = MATERIAL_EXTENSION;
		break;

	case RESOURCE_TYPE::TEXTURE:
		directory = TEXTURES_PATH;
		extension = TEXTURE_EXTENSION;
		break;
	}
	
	library_path = directory + file + extension;
	library_file = file + extension;

	directory.clear();
	file.clear();
	extension.clear();
}
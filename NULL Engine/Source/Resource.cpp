#include "Random.h"
#include "VarTypedefs.h"

#include "Resource.h"

Resource::Resource() : 
id				(Random::LCG::GetRandomUint()), 
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
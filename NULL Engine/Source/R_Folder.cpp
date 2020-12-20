#include "JSONParser.h"

#include "R_Folder.h"

R_Folder::R_Folder() : Resource(RESOURCE_TYPE::FOLDER)
{

}

R_Folder::~R_Folder()
{

}

bool R_Folder::CleanUp()
{
	bool ret = true;



	return ret;
}

bool R_Folder::SaveMeta(ParsonNode& meta_root) const
{
	bool ret = true;



	return ret;
}

bool R_Folder::LoadMeta(const ParsonNode& meta_root)
{
	bool ret = true;



	return ret;
}
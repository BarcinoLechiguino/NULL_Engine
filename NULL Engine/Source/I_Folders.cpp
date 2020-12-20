#include "JSONParser.h"

#include "FileSystemDefinitions.h"
#include "VariableTypedefs.h"

#include "Application.h"
#include "M_FileSystem.h"

#include "R_Folder.h"

#include "I_Folders.h"

uint64 Importer::Folders::Save(const R_Folder* r_folder, char** buffer)
{
	uint64 written = 0;
	
	if (r_folder == nullptr)
	{
		LOG("[ERROR] Importer: Could not Save R_Folder* in Library! Error: R_Folder* was nullptr.");
		return 0;
	}
	
	ParsonNode root_node = ParsonNode();
	ParsonArray contained_array = root_node.SetArray("ContainedResources");

	for (uint i = 0; i < r_folder->contained_resources.size(); ++i)
	{
		contained_array.SetNumber(r_folder->contained_resources[i]);
	}

	uint size = root_node.SerializeToBuffer(buffer);
	if (size == 0)
	{
		LOG("[ERROR] Importer: Could not Save R_Folder* in Library! Error: Could not Serialize Root Node to Buffer.");
		return 0;
	}

	std::string path = FOLDERS_PATH + std::to_string(r_folder->GetUID()) + FOLDERS_EXTENSION;
	written = App->file_system->Save(path.c_str(), *buffer, size);
	if (written > 0)
	{
		LOG("[IMPORTER] Importer: Successfully saved R_Folder* in Library! Path: %s", path.c_str());
	}
	else
	{
		LOG("[ERROR] Importer: Could not Save R_Folder* in Library! Error: File System could not write the file!");
	}

	path.clear();

	return written;
}

void Importer::Folders::Load(const char* buffer, R_Folder* r_folder)
{
	if (buffer == nullptr)
	{
		LOG("[ERROR] Importer: Could not Load R_Folder* from Library! Error: Given buffer was nullptr.");
		return;
	}
	
	ParsonNode root_node			= ParsonNode(buffer);
	ParsonArray contained_array		= root_node.GetArray("ContainedResources");
	if (!root_node.NodeIsValid())
	{
		LOG("[ERROR] Importer: Could not Load R_Folder* from Library! Error: Could not generate the Root Node from the passed buffer.");
		return;
	}
	if (!contained_array.ArrayIsValid())
	{
		LOG("[ERROR] Importer: Could not Load R_Folder* from Library! Error: Could not get the ModelNodes array from the Root Node.");
		return;
	}

	for (uint i = 0; i < contained_array.size; ++i)
	{
		uint32 contained_uid = (uint32)contained_array.GetNumber(i);
		r_folder->contained_resources.push_back(contained_uid);
	}

	LOG("[IMPORTER] Importer: Successfully Loaded R_Folder* from Library! UID: %lu", r_folder->GetUID());

	return;
}
#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <vector>
#include <map>
#include <string>

#include "Log.h"
#include "VarTypedefs.h"
#include "MathStructures.h"

struct ImporterSettings
{
	int a;
};

enum class RESOURCE_TYPE
{
	MESH,
	MATERIAL,
	TEXTURE
};

class Resource
{
public:
	Resource();
	virtual ~Resource();

	virtual bool CleanUp();

public:
	uint32				GetID				() const;
	void				SetID				();

	const char*			GetAssetsPath		() const;								// 
	const char*			GetAssetsFile		() const;								// 
	const char*			GetLibraryPath		() const;								// 
	const char*			GetLibraryFile		() const;								// 

	void				SetAssetsPath		(const char* assets_path);				// 
	void				SetAssetsFile		(const char* assets_file);				// 
	void				SetLibraryPath		(const char* library_path);				// 
	void				SetLibraryFile		(const char* library_file);				// 

	//ImporterSettings	GetImporterSettings	();
	//void				SetImporterSettings	(ImporterSettings importer_settings);

private:
	uint32				id;

	std::string			assets_path;												// Path of the file in the Assets directory. Will be used to avoid making duplicates.
	std::string			assets_file;												// File and extension string of the texture in the Assets directory.

	std::string			library_path;												// Path of the file in the Library directory. Will be used to avoid making duplicates.
	std::string			library_file;												// File and extension string of the texture in the Library directory.

	ImporterSettings	importer_settings;
};

#endif // !__RESOURCE_H__
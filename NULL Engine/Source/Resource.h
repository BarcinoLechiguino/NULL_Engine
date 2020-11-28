#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <vector>
#include <string>

#include "Log.h"
#include "VarTypedefs.h"
#include "MathStructures.h"

struct ImporterSettings
{
	int a;
};

class Resource
{
public:
	Resource();
	virtual ~Resource();

	virtual bool CleanUp();

public:
	//ImporterSettings GetImporterSettings();
	//void SetImporterSettings(ImporterSettings importer_settings);

private:
	ImporterSettings importer_settings;
};

#endif // !__RESOURCE_H__
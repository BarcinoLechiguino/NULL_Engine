#include "JSONParser.h"

#include "MaterialSettings.h"

MaterialSettings::MaterialSettings() : ImportSettings()
{

}

MaterialSettings::~MaterialSettings()
{

}

bool MaterialSettings::SaveImportSettings(ParsonNode& settings) const
{
	bool ret = true;


	
	return ret;
}

bool MaterialSettings::LoadImportSettings(const ParsonNode& settings)
{
	bool ret = true;



	return ret;
}
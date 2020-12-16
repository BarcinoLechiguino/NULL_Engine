#include "JSONParser.h"

#include "ModelSettings.h"

ModelSettings::ModelSettings() : ImportSettings()
{

}

ModelSettings::~ModelSettings()
{

}

bool ModelSettings::SaveImportSettings(ParsonNode& settings) const
{
	bool ret = true;


	
	return ret;
}

bool ModelSettings::LoadImportSettings(const ParsonNode& settings)
{
	bool ret = true;



	return ret;
}
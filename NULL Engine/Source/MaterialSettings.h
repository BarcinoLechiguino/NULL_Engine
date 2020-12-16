#ifndef __MATERIAL_SETTINGS_H__
#define __MATERIAL_SETTINGS_H__

#include "ImportSettings.h"

class ParsonNode;

class MaterialSettings : public ImportSettings
{
public:
	MaterialSettings();
	~MaterialSettings();

	bool SaveImportSettings(ParsonNode& settings) const override;
	bool LoadImportSettings(const ParsonNode& settings) override;

private:

};

#endif // !__MATERIAL_SETTINGS_H__
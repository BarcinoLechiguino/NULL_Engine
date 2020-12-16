#ifndef __MESH_SETTINGS_H__
#define __MESH_SETTINGS_H__

#include "ImportSettings.h"

class ParsonNode;

class MeshSettings : public ImportSettings
{
public:
	MeshSettings();
	~MeshSettings();

	bool SaveImportSettings(ParsonNode& settings) const override;
	bool LoadImportSettings(const ParsonNode& settings) override;


private:

};

#endif // !__MESH_SETTINGS_H__
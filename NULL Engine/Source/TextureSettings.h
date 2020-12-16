#ifndef __TEXTURE_SETTINGS_H__
#define __TEXTURE_SETTINGS_H__

#include "ImportSettings.h"

class ParsonNode;

class TextureSettings : public ImportSettings
{
public:
	TextureSettings();
	~TextureSettings();

	bool SaveImportSettings(ParsonNode& settings) const override;
	bool LoadImportSettings(const ParsonNode& settings) override;

private:

};

#endif // !__TEXTURE_SETTINGS_H__
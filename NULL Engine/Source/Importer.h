#ifndef __IMPORTER_H__
#define __IMPORTER_H__

#include "VarTypedefs.h"

enum class AXIS_ORIENTATION
{
	X_UP,
	Y_UP,
	Z_UP
};

struct ImportSettings
{
	ImportSettings();
	
	float				global_scale;
	AXIS_ORIENTATION	axis;
	bool				ignore_cameras;
	bool				ignore_lights;

	int					compression;
	bool				flip_X;
	bool				flip_Y;
	int					wrapping;
	int					filter;
	bool				generate_mipmaps;
	bool				anisotropy;
};

namespace Importer
{
	static ImportSettings import_settings;
	
	ImportSettings GetImporterSettings();
	void SetImporterSettings(const ImportSettings& importer_settings);
}

#endif // !__IMPORTER_H__
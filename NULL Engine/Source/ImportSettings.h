#ifndef __IMPORT_SETTINGS_H__
#define __IMPORT_SETTINGS_H__

enum class AXIS_ORIENTATION
{
	X_UP,
	Y_UP,
	Z_UP
};

struct ImportSettings													// TMP. Brought from Importer.h
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

#endif // !__IMPORT_SETTINGS_H__

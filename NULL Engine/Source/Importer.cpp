#include "Importer.h"

ImportSettings Importer::GetImporterSettings()
{
	return Importer::import_settings;
}

void Importer::SetImporterSettings(const ImportSettings& importer_settings)
{
	Importer::import_settings = importer_settings;
}

ImportSettings::ImportSettings()
{
	global_scale		= 1.0f;
	axis				= AXIS_ORIENTATION::Y_UP;
	ignore_cameras		= false;
	ignore_lights		= false;

	compression			= 0x070A;													// 0x070A = IL_DXT5. DevIL Flag.
	flip_X				= false;
	flip_Y				= false;
	wrapping			= 0x2901;													// 0x2901 = GL_REPEAT. OpenGL Flag.
	filter				= 0x2601;													// 0x2601 = GL_LINEAR. OpenGL Flag.
	generate_mipmaps	= true;
	anisotropy			= true;
}
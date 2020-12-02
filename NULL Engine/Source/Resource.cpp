#include "Random.h"

#include "Resource.h"

Resource::Resource() : id(0), assets_path("NONE"), assets_file("NONE"), library_path("NONE"), library_file("NONE")
{
	SetID();
}

Resource::~Resource()
{

}

bool Resource::CleanUp()
{
	bool ret = true;

	assets_path.clear();
	assets_file.clear();
	library_path.clear();
	library_file.clear();

	return ret;
}

// --- RESOURCE METHODS ---
uint32 Resource::GetID() const
{
	return id;
}

void Resource::SetID()
{
	id = Random::LCG::GetRandomUint();
}

const char* Resource::GetAssetsPath() const
{
	return assets_path.c_str();
}
const char* Resource::GetAssetsFile() const
{
	return assets_file.c_str();
}
const char* Resource::GetLibraryPath() const
{
	return library_path.c_str();
}
const char* Resource::GetLibraryFile() const
{
	return library_file.c_str();
}

void Resource::SetAssetsPath(const char* assets_path)
{
	this->assets_path = assets_path;
}

void Resource::SetAssetsFile(const char* assets_file)
{
	this->assets_file = assets_file;
}

void Resource::SetLibraryPath(const char* library_path)
{
	this->library_path = library_path;
}

void Resource::SetLibraryFile(const char* library_file)
{
	this->library_file = library_file;
}

// --- IMPORT SETTINGS ---
ImportSettings::ImportSettings()
{
	global_scale = 1.0f;
	axis = AXIS_ORIENTATION::Y_UP;
	ignore_cameras = false;
	ignore_lights = false;

	compression = 0x070A;													// 0x070A = IL_DXT5. DevIL Flag.
	flip_X = false;
	flip_Y = false;
	wrapping = 0x2901;													// 0x2901 = GL_REPEAT. OpenGL Flag.
	filter = 0x2601;													// 0x2601 = GL_LINEAR. OpenGL Flag.
	generate_mipmaps = true;
	anisotropy = true;
}
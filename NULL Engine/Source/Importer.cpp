#include <string>
#include <vector>

#include "Globals.h"

#include "Application.h"
#include "M_FileSystem.h"
#include "M_Scene.h"

#include "Resource.h"
#include "R_Texture.h"

#include "I_Scenes.h"
#include "I_Meshes.h"
#include "I_Materials.h"
#include "I_Textures.h"

#include "Importer.h"

using namespace Importer;																						// Not a good thing to do but it will be employed only inside this .cpp file.

void Importer::InitializeImporters()
{

}

void Importer::ShutDownImporters()
{

}

bool Importer::ImportFile(const char* path)
{
	bool ret = true;

	path = Utilities::GetValidPath(path);

	if (path == nullptr)
	{
		LOG("[ERROR] Dropped file was not located inside the Assets directory!");
		return false;
	}

	if (Utilities::FileHasModelExtension(path))
	{
		Utilities::ImportScene(path);
	}
	else if (Utilities::FileHasTextureExtension(path))
	{
		Utilities::ImportTexture(path);
	}
	else
	{
		LOG("[ERROR] Importer: Could not import the dropped file! Error: File extension is not supported!");
		ret = false;
	}

	return ret;
}

bool Importer::Utilities::ImportScene(const char* path)
{
	Importer::Scenes::Import(path, *App->scene->GetGameObjects());

	return true;
}

bool Importer::Utilities::ImportTexture(const char* path)
{
	bool ret = true;
	
	R_Texture* r_texture = new R_Texture();
	Importer::Textures::Import(path, r_texture);

	if (r_texture != nullptr && r_texture->GetTextureID() != 0)
	{
		ret = App->scene->ApplyNewTextureToSelectedGameObject(r_texture);
	}
	else
	{
		LOG("[ERROR] Importer: Could not import the dropped texture!");
		RELEASE(r_texture);
		ret = false;
	}

	return ret;
}

const char* Importer::Utilities::GetValidPath(const char* path)
{
	std::string norm_path = App->file_system->NormalizePath(path);

	uint dir_path_start = norm_path.find("Assets");
	if (dir_path_start != std::string::npos)
	{
		norm_path	= norm_path.substr(dir_path_start, norm_path.size());
		path		= _strdup(norm_path.c_str());
	}
	else
	{
		path = nullptr;
	}

	norm_path.clear();

	return path;
}

bool Importer::Utilities::FileHasModelExtension(const char* path)
{
	std::string extension = App->file_system->GetFileExtension(path);

	return (extension == "fbx" || extension == "FBX" 
			|| extension == "obj" || extension == "OBJ");
}

bool Importer::Utilities::FileHasTextureExtension(const char* path)
{
	std::string extension = App->file_system->GetFileExtension(path);

	return (extension == "png" || extension == "PNG" 
			|| extension == "tga" || extension == "TGA" 
			|| extension == "dds" || extension == "DDS");
}

/*ImportSettings Importer::Settings::GetImporterSettings()
{
	return Importer::Settings::import_settings;
}

void Importer::Settings::SetImporterSettings(const ImportSettings& importer_settings)
{
	Importer::Settings::import_settings = importer_settings;
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
}*/
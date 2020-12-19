// ----------------------------------------------------
// I_Materials.cpp --- Importing materials from files.
// Interface class between the engine and Assimp.
// ----------------------------------------------------

#include "OpenGL.h"
#include "Assimp.h"

#include "VariableTypedefs.h"
#include "Log.h"
#include "Color.h"

#include "Application.h"
#include "M_FileSystem.h"
#include "R_Material.h"
#include "R_Texture.h"

#include "I_Textures.h"

#include "I_Materials.h"

#include "MemoryManager.h"

using namespace Importer::Materials;																	// Not a good thing to do but it will be employed sparsely and only inside this .cpp

void Importer::Materials::Import(const aiMaterial* ai_material, R_Material* r_material)
{
	std::string dir_path	= ASSETS_TEXTURES_PATH + App->file_system->GetLastDirectory(r_material->GetAssetsPath());			// Dirty setting of the assets path.
	std::string file		= "";
	std::string full_path	= "";
	
	aiColor4D color;
	if (ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)															// Could also get specular and ambient occlusion colours.
	{
		r_material->diffuse_color = Color(color.r, color.g, color.b, color.a);
	}

	aiString tex_path;
	if (ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &tex_path) == AI_SUCCESS)												// At the moment only DIFFUSE textures will be imported.
	{
		file		= App->file_system->GetFileAndExtension(tex_path.C_Str());
		full_path	= dir_path + file;
		r_material->materials.push_back(Material(TEXTURE_TYPE::DIFFUSE, 0, full_path));
		//r_material->texture_id = Importer::Textures::Import(full_path.c_str(), r_texture);									// Redundant: r_material and r_texture will store the tex_id.
	}
	if (ai_material->GetTexture(aiTextureType_SPECULAR, 0, &tex_path) == AI_SUCCESS)											// Checking if there is a SPECULAR texture.
	{
		file		= App->file_system->GetFileAndExtension(tex_path.C_Str());
		full_path	= dir_path + file;
		r_material->materials.push_back(Material(TEXTURE_TYPE::SPECULAR, 0, full_path));
	}
	if (ai_material->GetTexture(aiTextureType_AMBIENT, 0, &tex_path) == AI_SUCCESS)												// Checking if there is a AMBIENT texture.
	{
		file		= App->file_system->GetFileAndExtension(tex_path.C_Str());
		full_path	= dir_path + file;
		r_material->materials.push_back(Material(TEXTURE_TYPE::AMBIENT, 0, full_path));
	}
	if (ai_material->GetTexture(aiTextureType_EMISSIVE, 0, &tex_path) == AI_SUCCESS)											// Checking if there is a EMISSIVE texture.
	{
		file		= App->file_system->GetFileAndExtension(tex_path.C_Str());
		full_path	= dir_path + file;
		r_material->materials.push_back(Material(TEXTURE_TYPE::EMISSIVE, 0, full_path));
	}
	if (ai_material->GetTexture(aiTextureType_HEIGHT, 0, &tex_path) == AI_SUCCESS)												// Checking if there is a HEIGHT texture.
	{
		file		= App->file_system->GetFileAndExtension(tex_path.C_Str());
		full_path	= dir_path + file;
		r_material->materials.push_back(Material(TEXTURE_TYPE::HEIGHT, 0, full_path));
	}
	if (ai_material->GetTexture(aiTextureType_NORMALS, 0, &tex_path) == AI_SUCCESS)												// Checking if there is a NORMALS texture.
	{
		file		= App->file_system->GetFileAndExtension(tex_path.C_Str());
		full_path	= dir_path + file;
		r_material->materials.push_back(Material(TEXTURE_TYPE::NORMALS, 0, full_path));
	}

	char* buffer = nullptr;
	uint64 written = Importer::Materials::Save(r_material, &buffer);
	if (written > 0)
	{
		bool success = Importer::Materials::Load(buffer, r_material);

		if (success)
		{
			LOG("[IMPORTER] Successfully Loaded Material From Library!");
		}
		else
		{
			LOG("[ERROR] Could not load Material from Library!");
		}
	}

	RELEASE_ARRAY(buffer);

	dir_path.clear();
	file.clear();
	full_path.clear();
}

uint64 Importer::Materials::Save(const R_Material* r_material, char** buffer)
{
	uint64 written = 0;
	
	if (r_material == nullptr)
	{
		return 0;
	}
	
	float color[4] = {
		r_material->diffuse_color.r,
		r_material->diffuse_color.g,
		r_material->diffuse_color.b,
		r_material->diffuse_color.a,
	};

	uint size = sizeof(color);

	if (size == 0)
	{
		return 0;
	}

	*buffer			= new char[size];
	char* cursor	= *buffer;
	uint bytes		= 0;

	// --- COLOR DATA ---
	bytes = sizeof(color);
	memcpy_s(cursor, size, color, bytes);
	cursor += bytes;

	// --- SAVING THE BUFFER ---
	std::string path = std::string(MATERIALS_PATH) + std::to_string(r_material->GetUID()) + std::string(MATERIALS_EXTENSION);

	written = App->file_system->Save(path.c_str(), *buffer, size);
	if (written > 0)
	{
		LOG("[IMPORTER] Importer Materials: Successfully Saved Material into Library!");
	}
	else
	{
		LOG("[ERROR] Importer Materials: Could not Save Material into Library! File System could did not write anything!");
	}

	return written;
}

bool Importer::Materials::Load(const char* buffer, R_Material* r_material)
{
	bool ret = true;
	
	if (buffer == nullptr || r_material == nullptr)
	{
		LOG("[ERROR] Materials Importer: Could not Load Material from Library, buffer and/or r_material were nullptr!");
	}

	char* cursor	= (char*)buffer;
	uint bytes		= 0;
	
	float color[4];
	
	bytes = sizeof(color);
	memcpy_s(color, bytes, cursor, bytes);
	cursor += bytes;

	r_material->diffuse_color.Set(color[0], color[1], color[2], color[3]);

	return ret;
}
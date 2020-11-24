// ----------------------------------------------------
// I_Materials.cpp --- Importing materials from files.
// Interface class between the engine and Assimp.
// ----------------------------------------------------

#include "OpenGL.h"
#include "Assimp.h"
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

// Importing, saving and loading aiMaterials with Assimp.
void Importer::Materials::Import(const char* path, const aiMaterial* ai_material, R_Material* r_material, R_Texture* r_texture)		// R_Texture* TMP until the Resource Manager is in place.
{
	std::string dir_path; 
	App->file_system->SplitFilePath(path, &dir_path, nullptr, nullptr);
	
	aiColor4D color;
	if (ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)													// Could also get specular and ambient occlusion colours.
	{
		r_material->diffuse_color = Color(color.r, color.g, color.b, color.a);
	}

	aiString tex_path;
	if (ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &tex_path) == AI_SUCCESS)										// At the moment only DIFFUSE textures will be imported.
	{
		std::string file = App->file_system->GetFileAndExtension(tex_path.C_Str());
		dir_path += file;
		//std::string dir_path = ASSET_TEXTURES_PATH;																	// Dirty setting of the assets path.
		//dir_path += file;
		
		r_material->texture_id = Importer::Textures::Import(dir_path.c_str(), r_texture);								// Redundant: r_material and r_texture will store the tex_id.
	}
}

uint64 Importer::Materials::Save(const R_Material* r_material, char** buffer)
{
	uint64 size = 0;
	
	return size;
}

void Importer::Materials::Load(const char* buffer, R_Material* r_material)
{

}
#ifndef __I_MATERIALS_H__
#define __I_MATERIALS_H__

#include "VarTypedefs.h"

struct aiScene;
struct aiMaterial;

class R_Material;

namespace Importer
{
	namespace Materials
	{
		void Import(const aiMaterial* ai_material, R_Material* r_material);		// Processes aiMaterial data into a ready-to-use R_Material. Returns nullptr on error.

		uint64 Save(const R_Material* r_material, char** buffer);				// Processes R_Material data into a ready-to-save buffer. Returns buffer size (0 on error).

		void Load(const char* buffer, R_Material* r_material);					// Processes buffer data into a ready-to-use R_Material. Returns nullptr on error.
	}
}

#endif // !__I_MATERIALS_H__
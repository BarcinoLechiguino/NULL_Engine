#ifndef __I_MATERIALS_H__
#define __I_MATERIALS_H__

struct aiScene;
struct aiMaterial;

class R_Material;

namespace Importer
{
	namespace Materials
	{
		namespace Assimp
		{
			void Import(const aiMaterial* ai_material, R_Material* r_material);		// Processes aiMaterial data into a ready-to-use R_Material. Returns nullptr on error.

			uint64 Save(const R_Material* r_material, char** buffer);				// Processes R_Material data into a ready-to-save buffer. Returns buffer size (0 on error).

			void Load(const char* buffer, R_Material* r_material);					// Processes buffer data into a ready-to-use R_Material. Returns nullptr on error.
		}

		namespace DevIL
		{
			void Init();

			void CleanUp();

			bool Import(const char* path, R_Material* r_material);
			//bool Import(const char* buffer, uint size, R_Material* r_material);

			uint CreateTexture(const void* data, uint width,
												 uint height,
												 int internal_format = 0x1908,		// 0x1908 = GL_RGBA
												 uint format = 0x1908,				// 0x1908 = GL_RGBA
												 uint target = 0x0DE1,				// 0x0DE1 = GL_TEXTURE_2D
												 int filter_type = 0x2600,			// 0x2600 = GL_NEAREST
												 int filling_type = 0x2901);		// 0x2901 = GL_REPEAT

			uint64 Save(const R_Material* r_material, char** buffer);

			void Load(const char* buffer, uint size, R_Material* r_material);
		}
	}
}

#endif // !__I_MATERIALS_H__
#ifndef __I_TEXTURES_H__
#define __I_TEXTURES_H__

#include <string>
#include "VarTypedefs.h"

class R_Texture;

namespace Importer
{
	namespace Textures
	{	
		uint	Import(const char* path, R_Texture* r_texture);

		uint64	Save(const R_Texture* r_texture, char** buffer);

		void	Load(const char* buffer, const uint size, R_Texture* r_texture);

		namespace Utilities
		{
			void Init();
			void CleanUp();
			
			uint CreateTexture(const void* data, uint width,
												 uint height,
												 uint target = 0x0DE1, 				// 0x0DE1 = GL_TEXTURE_2D
												 int filter_type = 0x2600,			// 0x2600 = GL_NEAREST
												 int filling_type = 0x2901,			// 0x2901 = GL_REPEAT
												 int internal_format = 0x1908,		// 0x1908 = GL_RGBA
												 uint format = 0x1908); 			// 0x1908 = GL_RGBA
		}
	}
}

#endif // !__I_TEXTURES_H__
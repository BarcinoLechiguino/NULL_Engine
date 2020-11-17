#ifndef __I_TEXTURES_H__
#define __I_TEXTURES_H__

#include "VarTypedefs.h"

class R_Texture;

namespace Importer
{
	namespace Textures
	{	
		void	Import(const char* path, R_Texture* r_texture);

		uint64	Save(const R_Texture* r_texture, char** buffer);

		void	Load(const char* buffer, R_Texture* r_texture);

		namespace Utilities
		{
			void Init();
			void CleanUp();
			
			void CreateTexture(const void* data, uint width,
												 uint height, 
												 int internal_format = 0x1908,		// 0x1908 = GL_RGBA
												 uint format = 0x1908, 				// 0x1908 = GL_RGBA
												 uint target = 0x0DE1, 				// 0x0DE1 = GL_TEXTURE_2D
												 int filter_type = 0x2600,			// 0x2600 = GL_NEAREST
												 int filling_type = 0x2901);		// 0x2901 = GL_REPEAT
		}
	}
}

#endif // !__I_TEXTURES_H__
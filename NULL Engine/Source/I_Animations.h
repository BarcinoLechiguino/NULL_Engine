#ifndef __I_ANIMATIONS_H__
#define __I_ANIMATIONS_H__

struct aiAnimation;
struct aiBone;

class R_Animation;

typedef unsigned int uint;

namespace Importer
{
	namespace Animations
	{
		void Import	(aiAnimation* ai_animation, R_Animation* r_animation);
		uint Save	(const R_Animation* r_animation, char** buffer);
		bool Load	(const char* buffer, R_Animation* r_animation);
	}
}

#endif // !__I_ANIMATIONS_H__
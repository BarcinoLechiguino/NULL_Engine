#ifndef __ANIMATOR_CLIP_H__
#define __ANIMATOR_CLIP_H__

#include <string>

class R_Animation;

typedef unsigned int uint;

struct AnimatorClip
{
	AnimatorClip() : root(nullptr), name("[NONE]"), start(0), end(0) {}
	AnimatorClip(const R_Animation* root, const std::string& name, const uint& start, const uint& end) : root(root), name(name), start(start), end(end) {}

	const R_Animation*	root;
	std::string			name;
	uint				start;
	uint				end;
};

#endif // !__ANIMATOR_CLIP_H__
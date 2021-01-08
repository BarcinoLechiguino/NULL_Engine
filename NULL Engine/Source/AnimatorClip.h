#ifndef __ANIMATOR_CLIP_H__
#define __ANIMATOR_CLIP_H__

#include <string>

class ParsonNode;
class R_Animation;

typedef unsigned int uint;

class AnimatorClip
{
public:
	AnimatorClip();
	AnimatorClip(const R_Animation* animation, const std::string& name, const uint& start, const uint& end, const bool& loop);

	bool StepClip	(const float& dt);
	void ClearClip	();

	bool SaveState(ParsonNode& root) const;
	bool LoadState(const ParsonNode& root);

public:																																// --- CLIP DEBUG METHODS
	void				StepClipToPrevKeyframe		();
	void				StepClipToNextKeyframe		();

public:																																// --- CLIP TIME DATA GET/SET METHODS
	void				LoopClip					(const bool& set_to);
	bool				IsLooped					() const;

	float				GetClipTime					() const;
	float				GetClipFrame				() const;
	uint				GetClipTick					() const;

	float				GetAnimationFrame			() const;
	uint				GetAnimationTick			() const;

public:																																// --- GENERAL CLIP DATA GET METHODS
	const R_Animation*	GetAnimation				() const;
	const char*			GetAnimationName			() const;
	float				GetAnimationTicksPerSecond	() const;
	float				GetAnimationDuration		() const;

	const char*			GetName						() const;
	uint				GetStart					() const;
	uint				GetEnd						() const;
	float				GetDuration					() const;
	float				GetDurationInSeconds		() const;

public:
	bool				playing;

private:
	const R_Animation*	animation;
	std::string			name;
	uint				start;
	uint				end;
	float				duration;
	float				duration_in_seconds;

	bool				loop;
	float				time;
	float				frame;
	uint				tick;
};

#endif // !__ANIMATOR_CLIP_H__
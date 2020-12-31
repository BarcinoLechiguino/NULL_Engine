#ifndef __C_ANIMATION_H__
#define __C_ANIMATION_H__

#include "Component.h"

class ParsonNode;

class GameObject;
class R_Animation;

class C_Animation : public Component
{
public:
	C_Animation(GameObject* owner);
	~C_Animation();

	bool Update() override;
	bool CleanUp() override;

	bool SaveState(ParsonNode& root) const override;
	bool LoadState(ParsonNode& root) override;

	static inline COMPONENT_TYPE GetType() { return COMPONENT_TYPE::ANIMATION; }

public:
	bool	Play				();
	bool	Pause				();
	bool	Step				();
	bool	Stop				();

	float	GetPlaybackSpeed	();
	void	SetPlaybackSpeed	();

	float	GetAnimationTime	();
	float	GetAnimationName	();
	float	GetTicksPerSecond	();
	float	GetDuration			();

private:
	std::vector<R_Animation*>	current_animations;
	std::vector<R_Animation*>	animations;
	
	bool	play;
	bool	pause;
	bool	step;
	bool	stop;
	
	float	playback_speed;
	float	animation_time;
};

#endif // !__C_ANIMATION_H__
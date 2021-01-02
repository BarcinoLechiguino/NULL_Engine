#ifndef __C_ANIMATION_H__
#define __C_ANIMATION_H__

#include <string>
#include <vector>
#include <map>

#include "Component.h"

namespace math
{
	class LineSegment;
}

class ParsonNode;
class Channel;

class GameObject;
class R_Animation;

typedef unsigned int uint;

class C_Animation : public Component
{
public:
	C_Animation(GameObject* owner);
	~C_Animation();

	bool Update		() override;
	bool CleanUp	() override;

	bool SaveState	(ParsonNode& root) const override;
	bool LoadState	(ParsonNode& root) override;

	static inline COMPONENT_TYPE GetType() { return COMPONENT_TYPE::ANIMATION; }

private:
	bool			StepAnimation				();
	bool			BlendAnimation				();

	void			FindCurrentAnimationBones	();

public:
	void			AddAnimation				(R_Animation* r_animation);
	
	bool			Play						();
	bool			Pause						();
	bool			Step						();
	bool			Stop						();

public:																											// --- CURRENT/BLENDING ANIMATION METHODS
	R_Animation*	GetCurrentAnimation			() const;
	R_Animation*	GetBlendingAnimation		() const;

	void			SetCurrentAnimation			(R_Animation* r_animation, uint index = 0);
	void			SetBlendingAnimation		(R_Animation* r_animation_to_blend_to, uint blend_frames);

	void			ClearCurrentAnimation		();
	void			ClearBlendingAnimation		();

public:																											// --- GET/SET METHODS
	float			GetPlaybackSpeed			() const;
	bool			GetAnimationLoop			() const;
	bool			GetPlayOnStart				() const;
	bool			GetCameraCulling			() const;

	float			GetAnimationTime			() const;
	const char*		GetAnimationName			() const;
	float			GetTicksPerSecond			() const;
	float			GetDuration					() const;

	void			SetPlaybackSpeed			(const float& playback_speed);
	void			SetAnimationLoop			(const bool& set_to);
	void			SetPlayOnStart				(const bool& set_to);
	void			SetCameraCulling			(const bool& set_to);

private:
	std::vector<R_Animation*>			animations;
	std::vector<LineSegment>			display_bones;
	std::map<std::string, GameObject*>	current_bones;

	R_Animation*	current_animation;
	R_Animation*	blending_animation;

	uint			blend_frames;

	bool			play;
	bool			pause;
	bool			step;
	bool			stop;

	float			animation_time;

	float			playback_speed;
	bool			loop_animation;
	bool			play_on_start;
	bool			camera_culling;
};

#endif // !__C_ANIMATION_H__
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
	void			UpdateDisplayBones			();
	void			GenerateBoneSegments		(GameObject* bone);

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
	std::vector<LineSegment> GetDisplayBones	() const;
	
	float			GetPlaybackSpeed			() const;
	bool			GetLoopAnimation			() const;
	bool			GetPlayOnStart				() const;
	bool			GetCameraCulling			() const;
	bool			GetShowBones				() const;

	const char*		GetAnimationName			() const;
	float			GetAnimationTime			() const;
	float			GetTicksPerSecond			() const;
	float			GetDuration					() const;

	void			SetPlaybackSpeed			(const float& playback_speed);
	void			SetLoopAnimation			(const bool& set_to);
	void			SetPlayOnStart				(const bool& set_to);
	void			SetCameraCulling			(const bool& set_to);
	void			SetShowBones				(const bool& set_to);

private:
	std::vector<R_Animation*>			animations;
	std::vector<LineSegment>			display_bones;
	std::map<std::string, GameObject*>	current_bones;

	R_Animation*	current_animation;
	R_Animation*	blending_animation;

	GameObject*		current_root_bone;

	uint			blend_frames;

	bool			play;
	bool			pause;
	bool			step;
	bool			stop;

	float			playback_speed;
	bool			loop_animation;
	bool			play_on_start;
	bool			camera_culling;
	
	bool			show_bones;
	
	float			animation_time;
};

#endif // !__C_ANIMATION_H__
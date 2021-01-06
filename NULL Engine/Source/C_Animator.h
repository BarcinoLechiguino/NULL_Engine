#ifndef __C_ANIMATOR_H__
#define __C_ANIMATOR_H__

#include <string>
#include <vector>
#include <map>

#include "MathGeoTransform.h"

#include "Channel.h"
#include "AnimatorClip.h"

#include "Component.h"

namespace math
{
	class LineSegment;
}

class ParsonNode;

class GameObject;
class R_Animation;

typedef unsigned int uint;
typedef unsigned __int32 uint32;

struct BoneLink
{
	BoneLink();
	BoneLink(const Channel& channel, GameObject* game_object);
	
	Channel channel;
	GameObject* game_object;
};

struct AnimationBones
{
	std::string name;
	std::vector<BoneLink> bones;
};

enum class INTERPOLATION_TYPE																								// WIP IDEA
{
	LINEAL,
	CUSTOM
};

class C_Animator : public Component																							// In charge of managing Skeletal Animation
{
public:
	C_Animator(GameObject* owner);
	~C_Animator();

	bool Update		() override;
	bool CleanUp	() override;

	bool SaveState	(ParsonNode& root) const override;
	bool LoadState	(ParsonNode& root) override;

	static inline COMPONENT_TYPE GetType() { return COMPONENT_TYPE::ANIMATOR; }

public:
	void				AddAnimation				(R_Animation* r_animation);
	void				AddClip						(const AnimatorClip& clip);
	void				PlayClip					(std::string clip_name, uint blend_frames);
	
	bool				Play						();
	bool				Pause						();
	bool				Step						();
	bool				Stop						();

public:																														// --- DEBUG METHODS
	bool				StepToPrevKeyframe			();
	bool				StepToNextKeyframe			();
	bool				RefreshBoneDisplay			();

public:																														// --- CURRENT/BLENDING ANIMATION METHODS
	AnimatorClip*		GetCurrentClip				() const;
	AnimatorClip*		GetBlendingClip				() const;
	
	void				SetCurrentClip				(AnimatorClip* clip);
	void				SetBlendingClip				(AnimatorClip* clip, uint blend_frames);

	void				ClearCurrentClip			();
	void				ClearBlendingClip			();

public:																														// --- GET/SET METHODS
	std::vector<LineSegment> GetDisplayBones		() const;

	float				GetCurrentClipTime			() const;
	float				GetCurrentClipFrame			() const;
	uint				GetCurrentClipTick			() const;

	const char*			GetCurrentClipAnimationName				() const;
	float				GetCurrentClipAnimationTicksPerSecond	() const;
	float				GetCurrentClipAnimationDuration			() const;

	const char*			GetCurrentClipName			() const;
	uint				GetCurrentClipStart			() const;
	uint				GetCurrentClipEnd			() const;
	float				GetCurrentClipDuration		() const;
	float				GetCurrentClipDurationInSec	() const;

	float				GetPlaybackSpeed			() const;
	bool				GetInterpolate				() const;
	bool				GetLoopAnimation			() const;
	bool				GetPlayOnStart				() const;
	bool				GetCameraCulling			() const;
	bool				GetShowBones				() const;

	void				SetPlaybackSpeed			(const float& playback_speed);
	void				SetInterpolate				(const bool& set_to);
	void				SetLoopAnimation			(const bool& set_to);
	void				SetPlayOnStart				(const bool& set_to);
	void				SetCameraCulling			(const bool& set_to);
	void				SetShowBones				(const bool& set_to);

private:																													// --- INTERNAL METHODS
	bool				StepAnimation				();
	bool				BlendAnimation				();

	const Transform		GetInterpolatedTransform	(const double& current_keyframe, const Channel& current_channel, const Transform& original_transform) const;
	const float3		GetInterpolatedPosition		(const double& current_keyframe, const Channel& current_channel, const float3& original_position) const;
	const Quat			GetInterpolatedRotation		(const double& current_keyframe, const Channel& current_channel, const Quat& original_rotation) const;
	const float3		GetInterpolatedScale		(const double& current_keyframe, const Channel& current_channel, const float3& original_scale) const;

	const Transform		GetPoseToPoseTransform		(const uint& current_tick, const Channel& current_channel, const Transform& original_transform) const;
	const float3		GetPoseToPosePosition		(const uint& current_tick, const Channel& current_channel, const float3& original_position) const;
	const Quat			GetPoseToPoseRotation		(const uint& current_tick, const Channel& current_channel, const Quat& original_rotation) const;
	const float3		GetPoseToPoseScale			(const uint& current_tick, const Channel& current_channel, const float3& original_scale) const;

	void				FindAnimationBones			(const R_Animation* r_animation);
	bool				FindBoneLinks				(const R_Animation* r_animation, std::vector<BoneLink>& links);
	GameObject*			FindRootBone				(const std::vector<BoneLink>& links);
	void				SetRootBone					(const GameObject* root_bone);

	void				UpdateDisplayBones			();
	void				GenerateBoneSegments		(const GameObject* bone);
	
	bool				GenerateDefaultClip			(const R_Animation* r_animation, AnimatorClip& default_clip);

	void				SortBoneLinksByHierarchy	(const std::vector<BoneLink>& bone_links, const GameObject* root_bone, std::vector<BoneLink>& sorted);

private:
	std::vector<R_Animation*>					animations;																	// Animation Resources. Contain bone information (transforms...).
	std::map<uint32, std::vector<BoneLink>>		animation_bones;
	std::map<std::string, AnimatorClip>			clips;																		// Segments of animations. "Idle", "Walk", "Attack"...
	
	std::vector<BoneLink>		current_bones;																				// Multiple animations will have the same bones.
	std::vector<BoneLink>		blending_bones;

	std::vector<LineSegment>	display_bones;																				// Line Segments between GO bones. For debug purposes.

	AnimatorClip*		current_clip;
	AnimatorClip*		blending_clip;

	const GameObject*	current_root_bone;

private:																													// --- FUNCTIONALITY VARIABLES
	uint			blend_frames;

	bool			play;
	bool			pause;
	bool			step;
	bool			stop;

	float			clip_time;
	float			clip_frame;
	uint			clip_tick;

private:																													// --- GET/SET VARIABLES	
	float			playback_speed;
	bool			interpolate;
	bool			loop_animation;
	bool			play_on_start;
	bool			camera_culling;
	
	bool			show_bones;
};

#endif // !__C_ANIMATOR_H__
#include "MathGeoLib/include/Geometry/LineSegment.h"

#include "Profiler.h"

#include "Time.h"

#include "Channel.h"
#include "AnimatorClip.h"

#include "Application.h"
#include "M_ResourceManager.h"

#include "R_Animation.h"

#include "GameObject.h"
#include "C_Transform.h"

#include "C_Animator.h"

typedef std::map<double, float3>::const_iterator	PositionKeyframe;
typedef std::map<double, Quat>::const_iterator		RotationKeyframe;
typedef std::map<double, float3>::const_iterator	ScaleKeyframe;

C_Animator::C_Animator(GameObject* owner) : Component(owner, COMPONENT_TYPE::ANIMATOR),
current_clip		(nullptr),
blending_clip		(nullptr),
current_root_bone	(nullptr)
{
	blend_frames		= 0;

	play				= false;
	pause				= false;
	step				= false;
	stop				= true;

	clip_time			= 0.0f;
	clip_frame			= 0.0f;
	clip_tick			= 0;

	playback_speed		= 1.0f;
	interpolate			= true;
	loop_animation		= false;
	play_on_start		= true;
	camera_culling		= true;
	show_bones			= false;
}

C_Animator::~C_Animator()
{
	current_clip		= nullptr;
	blending_clip		= nullptr;
	current_root_bone	= nullptr;
}

bool C_Animator::Update()
{
	BROFILER_CATEGORY("Animation Component Update", Profiler::Color::DarkSlateBlue);
	
	bool ret = true;

	if (play || step)
	{
		if (current_clip != nullptr)
		{
			StepAnimation();
		}

		step = false;
	}

	return ret;
}

bool C_Animator::CleanUp()
{
	bool ret = true;

	for (uint i = 0; i < animations.size(); ++i)
	{
		App->resource_manager->FreeResource(animations[i]->GetUID());
	}

	animations.clear();
	animation_bones.clear();
	clips.clear();

	current_bones.clear();
	blending_bones.clear();

	display_bones.clear();

	return ret;
}

bool C_Animator::SaveState(ParsonNode& root) const
{
	bool ret = true;



	return ret;
}

bool C_Animator::LoadState(ParsonNode& root)
{
	bool ret = true;



	return ret;
}

// --- C_ANIMATION METHODS ---
bool C_Animator::StepAnimation()
{
	bool ret = true;

	clip_time		+= Time::Real::GetDT() * playback_speed;
	clip_frame		= clip_time * GetCurrentClipAnimationTicksPerSecond();

	uint prev_tick	= clip_tick;
	clip_tick		= (uint)floor(clip_frame);

	float animation_frame	= GetCurrentClipStart() + clip_frame;
	uint animation_tick		= GetCurrentClipStart() + clip_tick;

	for (auto bone = current_bones.cbegin(); bone != current_bones.cend(); ++bone)
	{
		C_Transform* c_transform = bone->game_object->GetComponent<C_Transform>();
		if (c_transform == nullptr)
		{
			LOG("[WARNING] Animation Component: GameObject { %s } did not have a Transform Component!", bone->game_object->GetName());
			continue;
		}

		const Transform& original_transform = Transform(c_transform->GetLocalTransform());
		
		if (interpolate)
		{
			const Transform& interpolated_transform = GetInterpolatedTransform(animation_frame, bone->channel, original_transform);
			c_transform->ImportTransform(interpolated_transform);
		}
		else
		{
			if (clip_tick != prev_tick)
			{
				const Transform& pose_to_pose_transform = GetPoseToPoseTransform(animation_tick, bone->channel, original_transform);
				c_transform->ImportTransform(pose_to_pose_transform);
			}
		}
	}

	if (blending_clip != nullptr)
	{
		ret = BlendAnimation();
		return ret;
	}

	UpdateDisplayBones();

	if (clip_time > GetCurrentClipDurationInSec())
	{
		clip_time		= 0.0f;
		clip_frame		= 0.0f;
		clip_tick		= 0;

		if (!loop_animation)
		{
			Stop();
		}
	}

	return ret;
}

bool C_Animator::BlendAnimation()
{
	bool ret = true;



	return ret;
}

const Transform C_Animator::GetInterpolatedTransform(const double& current_keyframe, const Channel& channel, const Transform& original_transform) const
{
	float3	interpolated_position	= GetInterpolatedPosition(current_keyframe, channel, original_transform.position);
	Quat	interpolated_rotation	= GetInterpolatedRotation(current_keyframe, channel, original_transform.rotation);
	float3	interpolated_scale		= GetInterpolatedScale(current_keyframe, channel, original_transform.scale);

	return Transform(interpolated_position, interpolated_rotation, interpolated_scale);
}

const float3 C_Animator::GetInterpolatedPosition(const double& current_keyframe, const Channel& channel, const float3& original_position) const
{
	if (!channel.HasPositionKeyframes())
	{
		return original_position;
	}

	PositionKeyframe prev_keyframe = channel.GetClosestPrevPositionKeyframe(current_keyframe);
	PositionKeyframe next_keyframe = channel.GetClosestNextPositionKeyframe(current_keyframe);

	float rate = (float)(current_keyframe / next_keyframe->first);
	float3 ret = (prev_keyframe == next_keyframe) ? prev_keyframe->second : prev_keyframe->second.Lerp(next_keyframe->second, rate);

	return ret;
}

const Quat C_Animator::GetInterpolatedRotation(const double& current_keyframe, const Channel& channel, const Quat& original_rotation) const
{
	if (!channel.HasRotationKeyframes())
	{
		return original_rotation;
	}

	RotationKeyframe prev_keyframe = channel.GetClosestPrevRotationKeyframe(current_keyframe);
	RotationKeyframe next_keyframe = channel.GetClosestNextRotationKeyframe(current_keyframe);

	float rate = (float)(current_keyframe / next_keyframe->first);
	Quat ret = (prev_keyframe == next_keyframe) ? prev_keyframe->second : prev_keyframe->second.Slerp(next_keyframe->second, rate);

	return ret;
}

const float3 C_Animator::GetInterpolatedScale(const double& current_keyframe, const Channel& channel, const float3& original_scale) const
{
	if (!channel.HasScaleKeyframes())
	{
		return original_scale;
	}

	ScaleKeyframe prev_keyframe = channel.GetClosestPrevScaleKeyframe(current_keyframe);
	ScaleKeyframe next_keyframe = channel.GetClosestNextScaleKeyframe(current_keyframe);

	float rate = (float)(current_keyframe / next_keyframe->first);
	float3 ret = (prev_keyframe == next_keyframe) ? prev_keyframe->second : prev_keyframe->second.Lerp(next_keyframe->second, rate);

	return ret;
}

const Transform C_Animator::GetPoseToPoseTransform(const uint& current_tick, const Channel& channel, const Transform& original_transform) const
{
	const float3& position	= GetPoseToPosePosition(current_tick, channel, original_transform.position);
	const Quat& rotation	= GetPoseToPoseRotation(current_tick, channel, original_transform.rotation);
	const float3& scale		= GetPoseToPoseScale(current_tick, channel, original_transform.scale);
	
	return Transform(position, rotation, scale);
}

const float3 C_Animator::GetPoseToPosePosition(const uint& current_tick, const Channel& channel, const float3& original_position) const
{
	if (!channel.HasPositionKeyframes()) 
	{ 
		return original_position; 
	}

	return channel.GetPositionKeyframe(current_tick)->second;
}

const Quat C_Animator::GetPoseToPoseRotation(const uint& current_tick, const Channel& channel, const Quat& original_rotation) const
{
	if (!channel.HasRotationKeyframes())
	{
		return original_rotation;
	}

	return channel.GetRotationKeyframe(current_tick)->second;
}

const float3 C_Animator::GetPoseToPoseScale(const uint& current_tick, const Channel& channel, const float3& original_scale) const
{
	if (!channel.HasScaleKeyframes())
	{
		return original_scale;
	}

	return channel.GetScaleKeyframe(current_tick)->second;
}

void C_Animator::FindAnimationBones(const R_Animation* r_animation)
{
	if (r_animation == nullptr)
	{
		return;
	}
	if (r_animation->channels.empty())
	{
		return;
	}

	std::vector<BoneLink> links;
	bool success = FindBoneLinks(r_animation, links);
	if (success)
	{
		animation_bones.emplace(r_animation->GetUID(), links);

		GameObject* root_bone = FindRootBone(links);
		if (root_bone != nullptr)
		{
			SetRootBone(root_bone);
			UpdateDisplayBones();
		}
	}
}

bool C_Animator::FindBoneLinks(const R_Animation* r_animation, std::vector<BoneLink>& links)
{
	if (r_animation == nullptr)
	{
		LOG("[ERROR] Animator Component: Could not find Bone Links! Error: Given R_Animation* was nullptr.");
		return false;
	}
	if (r_animation->channels.empty())
	{
		LOG("[ERROR] Animator Component: Could not find { %s }'s Bone Links! Error: R_Animation* had no channels.");
		return false;
	}
	if (this->GetOwner()->childs.empty())
	{
		LOG("[ERROR] Animator Component: Could not find { %s }'s Bone Links! Error: Component Owner { %s } had no Childs.", this->GetOwner()->GetName());
		return false;
	} 
	
	std::map<std::string, GameObject*> childs;
	this->GetOwner()->GetAllChilds(childs);

	for (auto channel = r_animation->channels.cbegin(); channel != r_animation->channels.cend(); ++channel)						// Trying out the auto keyword
	{
		auto go_item = childs.find(channel->name);
		if (go_item != childs.end())
		{
			go_item->second->is_bone = true;
			links.push_back(BoneLink((*channel), go_item->second));
		}
	}

	childs.clear();

	return true;
}

GameObject* C_Animator::FindRootBone(const std::vector<BoneLink>& links)
{	
	for (auto link = links.cbegin(); link != links.cend(); ++link)																// Trying out the auto keyword
	{
		if (link->game_object->parent == nullptr)
		{
			continue;
		}

		if (!link->game_object->parent->is_bone)
		{
			return link->game_object;
		}
	}

	return nullptr;
}

void C_Animator::SetRootBone(const GameObject* root_bone)
{
	if (root_bone == nullptr)
	{
		LOG("[ERROR] Animator Component: Could not Set Root Bone! Error: Given GameObject* was nullptr.");
		return;
	}
	
	if (current_root_bone == nullptr)
	{
		current_root_bone = root_bone;
	}
	else
	{
		if (current_root_bone != root_bone)
		{
			LOG("[WARNING] Animator Component: Disparity between root bones detected! A: [%s], B: [%s]", current_root_bone->GetName(), root_bone->GetName());
		}
	}
}

void C_Animator::UpdateDisplayBones()
{
	display_bones.clear();
	
	if (current_root_bone != nullptr)
	{
		GenerateBoneSegments(current_root_bone);
	}

	return;
}

void C_Animator::GenerateBoneSegments(const GameObject* bone)
{
	if (bone == nullptr)
	{
		LOG("[ERROR] Animation Component: Could not Generate Bone Segments! Error: Given GameObject* was nullptr.");
		return;
	}
	if (bone->childs.empty() || !bone->is_bone)
	{
		return;
	}
	
	C_Transform* bone_transform = bone->GetComponent<C_Transform>();

	for (uint i = 0; i < bone->childs.size(); ++i)
	{
		LineSegment display_bone = { float3::zero, float3::zero };

		display_bone.a = bone_transform->GetWorldPosition();
		display_bone.b = bone->childs[i]->GetComponent<C_Transform>()->GetWorldPosition();

		display_bones.push_back(display_bone);

		GenerateBoneSegments(bone->childs[i]);
	}
}

bool C_Animator::GenerateDefaultClip(const R_Animation* r_animation, AnimatorClip& default_clip)
{
	if (r_animation == nullptr)
	{
		LOG("[ERROR] Animator Component: Could not Generate Default Clip! Error: Given R_Animation* was nullptr.");
		return false;
	}
	
	std::string default_name	= r_animation->GetName() + std::string(" Default");
	default_clip				= AnimatorClip(r_animation, default_name, 0, (uint)r_animation->GetDuration());

	return true;
}

void C_Animator::SortBoneLinksByHierarchy(const std::vector<BoneLink>& bone_links, const GameObject* root_bone, std::vector<BoneLink>& sorted)
{
	if (root_bone == nullptr)
	{
		return;
	}
	
	if (root_bone == current_root_bone)
	{
		for (uint j = 0; j < bone_links.size(); ++j)
		{
			if (bone_links[j].channel.name == root_bone->GetName())
			{
				sorted.push_back(bone_links[j]);
			}
		}
	}

	for (uint i = 0; i < root_bone->childs.size(); ++i)
	{
		for (uint j = 0; j < bone_links.size(); ++j)
		{
			if (bone_links[j].channel.name == root_bone->childs[i]->GetName())
			{
				sorted.push_back(bone_links[j]);
			}
		}
	}

	for (uint i = 0; i < root_bone->childs.size(); ++i)
	{
		SortBoneLinksByHierarchy(bone_links, root_bone->childs[i], sorted);
	}
}

void C_Animator::AddAnimation(R_Animation* r_animation)
{
	if (r_animation == nullptr)
	{
		LOG("[ERROR] Animator Component: Could not Add Animation to %s's Animation Component! Error: Argument R_Animation* was nullptr.", this->GetOwner()->GetName());
		return;
	}

	animations.push_back(r_animation);

	FindAnimationBones(r_animation);

	AnimatorClip default_clip = AnimatorClip();
	bool success = GenerateDefaultClip(r_animation, default_clip);
	if (success)
	{
		clips.emplace(default_clip.name, default_clip);

		if (current_clip == nullptr)
		{
			SetCurrentClip(&clips.find(default_clip.name)->second);
		}
	}
}

bool C_Animator::AddClip(const AnimatorClip& clip)
{
	if (clip.animation == nullptr)
	{
		LOG("[ERROR] Animator Component: Could not Add Clip { %s }! Error: Clip's R_Animation* was nullptr.", clip.name.c_str());
		return false;
	}
	if (clips.find(clip.name) != clips.end())
	{
		LOG("[ERROR] Animator Component: Could not Add Clip { %s }! Error: A clip with the same name already exists.", clip.name.c_str());
		return false;
	}
	
	clips.emplace(clip.name, clip);

	if (current_clip == nullptr)
	{
		current_clip = (AnimatorClip*)&clip;
	}

	return true;
}

bool C_Animator::Play()
{
	play	= true;

	pause	= false;
	step	= false;
	stop	= false;

	return play;
}

bool C_Animator::Pause()
{
	if (play)
	{
		pause	= true;
		play	= false;
		step	= false;
	}
	else
	{
		LOG("[WARNING] Animation Component: Cannot Pause a Stopped Animation!");
	}

	return pause;
}

bool C_Animator::Step()
{	
	if (pause)
	{
		step = true;
	}
	else
	{
		LOG("[WARNING] Animation Component: Only Paused Animations can be Stepped!");
	}

	return step;
}

bool C_Animator::Stop()
{
	stop	= true;

	play	= false;
	pause	= false;
	step	= false;

	clip_time	= 0.0f;
	clip_frame	= 0.0f;
	clip_tick	= 0;
	
	return stop;
}

// --- DEBUG METHODS
bool C_Animator::StepToPrevKeyframe()
{
	if (play)
	{
		return false;
	}
	
	if (clip_tick != 0)
	{
		--clip_tick;
	}

	for (uint i = 0; i < current_bones.size(); ++i)
	{
		const Transform& transform				= Transform(current_bones[i].game_object->GetComponent<C_Transform>()->GetLocalTransform());
		const Transform& interpolated_transform = GetInterpolatedTransform((double)clip_tick, current_bones[i].channel, transform);

		current_bones[i].game_object->GetComponent<C_Transform>()->ImportTransform(interpolated_transform);
	}

	UpdateDisplayBones();

	return true;
}

bool C_Animator::StepToNextKeyframe()
{
	if (play)
	{
		return false;
	}
	
	if ((double)clip_tick < GetCurrentClipAnimationDuration())
	{
		++clip_tick;
	}
	
	for (uint i = 0; i < current_bones.size(); ++i)
	{
		const Transform& transform				= Transform(current_bones[i].game_object->GetComponent<C_Transform>()->GetLocalTransform());
		const Transform& interpolated_transform = GetInterpolatedTransform((double)clip_tick, current_bones[i].channel, transform);

		current_bones[i].game_object->GetComponent<C_Transform>()->ImportTransform(interpolated_transform);
	}

	UpdateDisplayBones();

	return false;
}

bool C_Animator::RefreshBoneDisplay()
{
	UpdateDisplayBones();

	return true;
}

// --- CURRENT/BLENDING ANIMATION METHODS
AnimatorClip* C_Animator::GetCurrentClip() const
{
	return current_clip;
}

AnimatorClip* C_Animator::GetBlendingClip() const
{
	return blending_clip;
}

void C_Animator::SetCurrentClip(AnimatorClip* clip)
{
	std::string error_string = "[ERROR] Animator Component: Could not Set Current Clip to { " + std::string(this->GetOwner()->GetName()) + " }'s Animator Component";
	
	if (clip == nullptr)
	{
		LOG("%s! Error: Given AnimatorClip* was nullptr.", error_string.c_str());
		return;
	}
	if (clips.find(clip->name) == clips.end())
	{
		LOG("%s! Error: Could not find the given AnimatorClip* in the clips map.", error_string.c_str());
		return;
	}
	if (animation_bones.find(clip->animation->GetUID()) == animation_bones.end())
	{
		LOG("%s! Error: Could not find the Bones of the Clip's animation (R_Animation*).");
		return;
	}

	current_clip	= clip;
	current_bones	= animation_bones.find(clip->animation->GetUID())->second;
}

void C_Animator::SetBlendingClip(AnimatorClip* clip, uint blend_frames)
{
	std::string error_string = "[ERROR] Animator Component: Could not Set Blending Clip to { " + std::string(this->GetOwner()->GetName()) + " }'s Animator Component";

	if (clip == nullptr)
	{
		LOG("%s! Error: Given AnimatorClip* was nullptr.", error_string.c_str());
		return;
	}
	if (clips.find(clip->name) == clips.end())
	{
		LOG("%s! Error: Could not find the given AnimatorClip* in the clips map.", error_string.c_str());
		return;
	}
	if (animation_bones.find(clip->animation->GetUID()) != animation_bones.end())
	{
		LOG("%s! Error: Could not find the Bones of the Clip's animation (R_Animation*).");
		return;
	}
	
	blending_clip		= clip;
	this->blend_frames	= blend_frames;
}

void C_Animator::SetCurrentClipByIndex(const uint& index)
{
	if (index >= clips.size())
	{
		LOG("[ERROR] Animator Component: Could not Set Current Clip By Index! Error: Given Index was out of bounds.");
		return;
	}

	// Cannot be accessed by index, only by name (key).
}

void C_Animator::SetBlendingClipByIndex(const uint& index, const uint& blend_frames)
{

}

void C_Animator::ClearCurrentClip()
{
	current_clip = nullptr;
	current_bones.clear();
}

void C_Animator::ClearBlendingClip()
{
	blending_clip = nullptr;
}

// --- GET/SET METHODS
std::vector<LineSegment> C_Animator::GetDisplayBones() const
{
	return display_bones;
}

R_Animation* C_Animator::GetAnimationByIndex(const uint& index) const
{
	if (index >= animations.size())
	{
		LOG("[ERROR] Animator Component: Could not get Animation by Index! Error: Given index was out of bounds.");
		return nullptr;
	}

	return animations[index];
}

std::string C_Animator::GetAnimationNamesAsString() const
{
	std::string animation_names = "";

	for (auto animation = animations.cbegin(); animation != animations.cend(); ++animation)
	{
		animation_names += (*animation)->GetName();
		animation_names += '\0';
	}

	return animation_names;
}

std::string C_Animator::GetClipNamesAsString() const
{
	std::string clip_names = "";
	
	for (auto clip = clips.cbegin(); clip != clips.cend(); ++clip)
	{
		clip_names += clip->first.c_str();
		clip_names += '\0';
	}

	return clip_names;
}

float C_Animator::GetCurrentClipTime() const
{
	return clip_time;
}

float C_Animator::GetCurrentClipFrame() const
{
	return clip_frame;
}

uint C_Animator::GetCurrentClipTick() const
{
	return clip_tick;
}

const char* C_Animator::GetCurrentClipAnimationName() const
{
	return ((current_clip == nullptr) ? "[NONE]" : current_clip->animation->GetName());
}

float C_Animator::GetCurrentClipAnimationTicksPerSecond() const
{
	return ((current_clip == nullptr) ? 0.0f : (float)current_clip->animation->GetTicksPerSecond());
}

float C_Animator::GetCurrentClipAnimationDuration() const
{
	return ((current_clip == nullptr) ? 0.0f : (float)current_clip->animation->GetDuration());
}

const char* C_Animator::GetCurrentClipName() const
{
	return ((current_clip == nullptr) ? "[NONE]" : current_clip->name.c_str());
}

uint C_Animator::GetCurrentClipStart() const
{
	return ((current_clip == nullptr) ? 0 : current_clip->start);
}

uint C_Animator::GetCurrentClipEnd() const
{
	return ((current_clip == nullptr) ? 0 : current_clip->end);
}

float C_Animator::GetCurrentClipDuration() const
{
	return ((current_clip == nullptr) ? 0.0f : (GetCurrentClipEnd() - GetCurrentClipStart()));
}

float C_Animator::GetCurrentClipDurationInSec() const
{
	return ((current_clip == nullptr) ? 0.0f : (GetCurrentClipDuration() / GetCurrentClipAnimationTicksPerSecond()));
}

float C_Animator::GetPlaybackSpeed() const
{
	return playback_speed;
}

bool C_Animator::GetInterpolate() const
{
	return interpolate;
}

bool C_Animator::GetLoopAnimation() const
{
	return loop_animation;
}

bool C_Animator::GetPlayOnStart() const
{
	return play_on_start;
}

bool C_Animator::GetCameraCulling() const
{
	return camera_culling;
}

bool C_Animator::GetShowBones() const
{
	return show_bones;
}

void C_Animator::SetPlaybackSpeed(const float& playback_speed)
{
	this->playback_speed = playback_speed;
}

void C_Animator::SetInterpolate(const bool& set_to)
{
	interpolate = set_to;
}

void C_Animator::SetLoopAnimation(const bool& set_to)
{
	loop_animation = set_to;
}

void C_Animator::SetPlayOnStart(const bool& set_to)
{
	play_on_start = set_to;
}

void C_Animator::SetCameraCulling(const bool& set_to)
{
	camera_culling = set_to;
}

void C_Animator::SetShowBones(const bool& set_to)
{
	show_bones = set_to;
}

// --- BONE LINK METHODS
BoneLink::BoneLink() : 
channel(Channel()),
game_object(nullptr)
{

}

BoneLink::BoneLink(const Channel& channel, GameObject* game_object) : 
channel(channel),
game_object(game_object)
{

}
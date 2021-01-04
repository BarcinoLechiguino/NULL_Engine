#include "MathGeoLib/include/Geometry/LineSegment.h"

#include "Profiler.h"

#include "Time.h"
#include "EasingFunctions.h"

#include "Channel.h"

#include "Application.h"
#include "M_ResourceManager.h"

#include "R_Animation.h"

#include "GameObject.h"
#include "C_Transform.h"

#include "C_Animation.h"

typedef std::map<double, float3>::const_iterator	PositionKeyframe;
typedef std::map<double, Quat>::const_iterator		RotationKeyframe;
typedef std::map<double, float3>::const_iterator	ScaleKeyframe;

C_Animation::C_Animation(GameObject* owner) : Component(owner, COMPONENT_TYPE::ANIMATION),
current_animation	(nullptr),
blending_animation	(nullptr),
current_root_bone	(nullptr)
{
	blend_frames		= 0;

	play				= false;
	pause				= false;
	step				= false;
	stop				= true;

	animation_time		= 0.0f;
	animation_frame		= 0.0f;
	animation_tick		= 0;

	playback_speed		= 1.0f;
	interpolate			= true;
	loop_animation		= false;
	play_on_start		= true;
	camera_culling		= true;
	show_bones			= false;
}

C_Animation::~C_Animation()
{
	current_animation	= nullptr;
	blending_animation	= nullptr;
	current_root_bone	= nullptr;
}

bool C_Animation::Update()
{
	BROFILER_CATEGORY("Animation Component Update", Profiler::Color::DarkSlateBlue);
	
	bool ret = true;

	if (play || step)
	{
		if (current_animation != nullptr)
		{
			StepAnimation();
		}

		step = false;
	}

	return ret;
}

bool C_Animation::CleanUp()
{
	bool ret = true;

	for (uint i = 0; i < animations.size(); ++i)
	{
		App->resource_manager->FreeResource(animations[i]->GetUID());
	}

	animations.clear();
	display_bones.clear();
	current_bones.clear();

	return ret;
}

bool C_Animation::SaveState(ParsonNode& root) const
{
	bool ret = true;



	return ret;
}

bool C_Animation::LoadState(ParsonNode& root)
{
	bool ret = true;



	return ret;
}

// --- C_ANIMATION METHODS ---
bool C_Animation::StepAnimation()
{
	bool ret = true;

	animation_time += Time::Real::GetDT() * playback_speed;
	animation_frame = animation_time * GetCurrentTicksPerSecond();

	uint prev_tick = animation_tick;
	animation_tick = (uint)floor(animation_frame);

	if (blending_animation != nullptr)
	{
		ret = BlendAnimation();
		return ret;
	}

	for (uint i = 0; i < current_bones.size(); ++i)
	{
		C_Transform* c_transform = current_bones[i].game_object->GetComponent<C_Transform>();
		if (c_transform == nullptr)
		{
			LOG("[WARNING] Animation Component: GameObject { %s } did not have a Transform Component!", current_bones[i].game_object->GetName());
			continue;
		}

		const Transform& original_transform = Transform(c_transform->GetLocalTransform());
		
		if (interpolate)
		{
			const Transform& interpolated_transform = GetInterpolatedTransform(animation_frame, current_bones[i].channel, original_transform);
			c_transform->ImportTransform(interpolated_transform);
		}
		else
		{
			// Pose To Pose Transform
			if (animation_tick != prev_tick)
			{
				const Transform& interpolated_transform = GetInterpolatedTransform(animation_tick, current_bones[i].channel, original_transform);
				c_transform->ImportTransform(interpolated_transform);
			}
		}
	}

	UpdateDisplayBones();

	if (animation_time > (GetCurrentDuration() / GetCurrentTicksPerSecond()))
	{
		animation_time		= 0.0f;
		animation_frame		= 0.0f;
		animation_tick		= 0;

		if (!loop_animation)
		{
			Stop();
		}
	}

	return ret;
}

bool C_Animation::BlendAnimation()
{
	bool ret = true;



	return ret;
}

void C_Animation::FindCurrentAnimationBones()
{
	if (current_animation == nullptr)
	{
		return;
	}
	if (this->GetOwner()->childs.empty())
	{
		return;
	}

	std::map<std::string, GameObject*> childs;
	this->GetOwner()->GetAllChilds(childs);

	std::map<std::string, GameObject*>::const_iterator go_item;
 	for (uint i = 0; i < current_animation->channels.size(); ++i)
	{
		go_item = childs.find(current_animation->channels[i].name);
		if (go_item != childs.end())
		{
			go_item->second->is_bone = true;
			current_bones.push_back(BoneLink(current_animation->channels[i], go_item->second));
		}
	}

	childs.clear();

	std::vector<BoneLink>::const_iterator item;
	for (item = current_bones.cbegin(); item != current_bones.cend(); ++item)
	{
		if (!item->game_object->parent->is_bone)
		{
			current_root_bone = item->game_object;
			break;
		}
	}

	UpdateDisplayBones();
}

void C_Animation::UpdateDisplayBones()
{
	display_bones.clear();
	
	if (current_root_bone != nullptr)
	{
		GenerateBoneSegments(current_root_bone);
	}

	return;
}

void C_Animation::GenerateBoneSegments(const GameObject* bone)
{
	if (bone == nullptr)
	{
		LOG("[ERROR] Animation Component: Could not Generate Bone Segments! Error: Given GameObject* was nullptr.");
		return;
	}
	if (bone->childs.empty())
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

void C_Animation::SortBoneLinksByHierarchy(const std::vector<BoneLink>& bone_links, const GameObject* root_bone, std::vector<BoneLink>& sorted)
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

const Transform C_Animation::GetInterpolatedTransform(const double& current_keyframe, const Channel& channel, const Transform& original_transform)
{
	float3	interpolated_position	= GetInterpolatedPosition(current_keyframe, channel, original_transform.position);
	Quat	interpolated_rotation	= GetInterpolatedRotation(current_keyframe, channel, original_transform.rotation);
	float3	interpolated_scale		= GetInterpolatedScale(current_keyframe, channel, original_transform.scale);

	return Transform(interpolated_position, interpolated_rotation, interpolated_scale);
}

const float3 C_Animation::GetInterpolatedPosition(const double& current_keyframe, const Channel& channel, const float3& original_position)
{
	if (!channel.HasPositionKeyframes())
	{
		return original_position;
	}
	
	PositionKeyframe prev_keyframe = channel.GetClosestPrevPositionKeyframe(current_keyframe);
	PositionKeyframe next_keyframe = channel.GetClosestNextPositionKeyframe(current_keyframe);

	float rate = (current_keyframe - prev_keyframe->first) / (next_keyframe->first - prev_keyframe->first);
	float3 ret = (prev_keyframe == next_keyframe) ? prev_keyframe->second : prev_keyframe->second.Lerp(next_keyframe->second, rate);
	
	return ret;
}

const Quat C_Animation::GetInterpolatedRotation(const double& current_keyframe, const Channel& channel, const Quat& original_rotation)
{
	if (!channel.HasRotationKeyframes())
	{
		return original_rotation;
	}
	
	RotationKeyframe prev_keyframe = channel.GetClosestPrevRotationKeyframe(current_keyframe);
	RotationKeyframe next_keyframe = channel.GetClosestNextRotationKeyframe(current_keyframe);

	float rate	= current_keyframe / next_keyframe->first;
	Quat ret	= (prev_keyframe == next_keyframe) ? prev_keyframe->second : prev_keyframe->second.Slerp(next_keyframe->second, rate);

	return ret;
}

const float3 C_Animation::GetInterpolatedScale(const double& current_keyframe, const Channel& channel, const float3& original_scale)
{
	if (!channel.HasScaleKeyframes())
	{
		return original_scale;
	}
	
	ScaleKeyframe prev_keyframe = channel.GetClosestPrevScaleKeyframe(current_keyframe);
	ScaleKeyframe next_keyframe = channel.GetClosestNextScaleKeyframe(current_keyframe);

	float rate = current_keyframe / next_keyframe->first;
	float3 ret = (prev_keyframe == next_keyframe) ? prev_keyframe->second : prev_keyframe->second.Lerp(next_keyframe->second, rate);
	
	return ret;
}

bool C_Animation::StepToPrevKeyframe()
{
	if (play)
	{
		return false;
	}
	
	if (animation_tick != 0)
	{
		--animation_tick;
	}

	for (uint i = 0; i < current_bones.size(); ++i)
	{
		const Transform& transform				= Transform(current_bones[i].game_object->GetComponent<C_Transform>()->GetLocalTransform());
		const Transform& interpolated_transform = GetInterpolatedTransform((double)animation_tick, current_bones[i].channel, transform);

		current_bones[i].game_object->GetComponent<C_Transform>()->ImportTransform(interpolated_transform);
	}

	UpdateDisplayBones();

	return true;
}

bool C_Animation::StepToNextKeyframe()
{
	if (play)
	{
		return false;
	}
	
	if ((double)animation_tick < GetCurrentDuration())
	{
		++animation_tick;
	}
	
	for (uint i = 0; i < current_bones.size(); ++i)
	{
		const Transform& transform				= Transform(current_bones[i].game_object->GetComponent<C_Transform>()->GetLocalTransform());
		const Transform& interpolated_transform = GetInterpolatedTransform((double)animation_tick, current_bones[i].channel, transform);

		current_bones[i].game_object->GetComponent<C_Transform>()->ImportTransform(interpolated_transform);
	}

	UpdateDisplayBones();

	return false;
}

bool C_Animation::RefreshBoneDisplay()
{
	UpdateDisplayBones();

	return true;
}

void C_Animation::AddAnimation(R_Animation* r_animation)
{
	if (r_animation == nullptr)
	{
		LOG("[ERROR] Component Animation: Could not Add Animation to %s's Animation Component! Error: Argument R_Animation* was nullptr.", this->GetOwner()->GetName());
		return;
	}

	animations.push_back(r_animation);

	if (current_animation == nullptr)
	{
		SetCurrentAnimation(r_animation);
	}
}

bool C_Animation::Play()
{
	play	= true;

	pause	= false;
	step	= false;
	stop	= false;

	return play;
}

bool C_Animation::Pause()
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

bool C_Animation::Step()
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

bool C_Animation::Stop()
{
	stop	= true;

	play	= false;
	pause	= false;
	step	= false;

	animation_time	= 0.0f;
	animation_frame	= 0.0f;
	animation_tick	= 0;
	
	return stop;
}

// --- CURRENT/BLENDING ANIMATION METHODS
R_Animation* C_Animation::GetCurrentAnimation() const
{
	return current_animation;
}

R_Animation* C_Animation::GetBlendingAnimation() const
{
	return blending_animation;
}

void C_Animation::SetCurrentAnimation(R_Animation* r_animation, uint index)
{
	if (r_animation == nullptr)
	{
		LOG("[ERROR] Animation Component: Could not Set Current Animation to %s's Animation Component! Error: Given R_Animation* was nullptr.", this->GetOwner()->GetName());
		return;
	}
	
	current_animation = r_animation;

	FindCurrentAnimationBones();
}

void C_Animation::SetBlendingAnimation(R_Animation* r_animation_to_blend_to, uint blend_frames)
{
	if (r_animation_to_blend_to == nullptr)
	{
		LOG("[ERROR] Animation Component: Could not Set Blending Animation to { %s }'s Animation Component! Error: Given R_Animation* was nullptr.", this->GetOwner()->GetName());
	}

	blending_animation = r_animation_to_blend_to;
	this->blend_frames = blend_frames;
}

void C_Animation::ClearCurrentAnimation()
{
	// Maybe On Clear other parameters need to be updated.
	
	current_animation = nullptr;
}

void C_Animation::ClearBlendingAnimation()
{
	blending_animation = nullptr;
}

// --- GET/SET METHODS
std::vector<LineSegment> C_Animation::GetDisplayBones() const
{
	return display_bones;
}

float C_Animation::GetPlaybackSpeed() const
{
	return playback_speed;
}

bool C_Animation::GetInterpolate() const
{
	return interpolate;
}

bool C_Animation::GetLoopAnimation() const
{
	return loop_animation;
}

bool C_Animation::GetPlayOnStart() const
{
	return play_on_start;
}

bool C_Animation::GetCameraCulling() const
{
	return camera_culling;
}

bool C_Animation::GetShowBones() const
{
	return show_bones;
}

const char* C_Animation::GetAnimationName() const
{
	return ((current_animation == nullptr) ? "[NONE]" : current_animation->GetName());
}

float C_Animation::GetAnimationTime() const
{
	return animation_time;
}

uint C_Animation::GetAnimationTicks() const
{
	return animation_tick;
}

float C_Animation::GetCurrentTicksPerSecond() const
{
	return ((current_animation == nullptr) ? 0.0f : current_animation->GetCurrentTicksPerSecond());
}

float C_Animation::GetCurrentDuration() const
{
	return ((current_animation == nullptr) ? 0.0f : current_animation->GetCurrentDuration());
}

void C_Animation::SetPlaybackSpeed(const float& playback_speed)
{
	this->playback_speed = playback_speed;
}

void C_Animation::SetInterpolate(const bool& set_to)
{
	interpolate = set_to;
}

void C_Animation::SetLoopAnimation(const bool& set_to)
{
	loop_animation = set_to;
}

void C_Animation::SetPlayOnStart(const bool& set_to)
{
	play_on_start = set_to;
}

void C_Animation::SetCameraCulling(const bool& set_to)
{
	camera_culling = set_to;
}

void C_Animation::SetShowBones(const bool& set_to)
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
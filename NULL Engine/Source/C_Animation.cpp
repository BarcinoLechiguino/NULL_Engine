#include "MathGeoLib/include/Geometry/LineSegment.h"

#include "Channel.h"

#include "Application.h"
#include "M_ResourceManager.h"

#include "R_Animation.h"

#include "GameObject.h"
#include "C_Transform.h"

#include "C_Animation.h"

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

	playback_speed		= 1.0f;
	animation_time		= 0.0f;

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
	bool ret = true;



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

 	for (uint i = 0; i < current_animation->channels.size(); ++i)
	{
		std::map<std::string, GameObject*>::const_iterator item;

		item = childs.find(current_animation->channels[i].name);
		if (item != childs.end())
		{
			item->second->is_bone = true;
			current_bones.emplace(item->first, item->second);
		}
	}

	childs.clear();

	std::map<std::string, GameObject*>::const_iterator item;
	for (item = current_bones.cbegin(); item != current_bones.cend(); ++item)
	{
		if (!item->second->parent->is_bone)
		{
			current_root_bone = item->second;
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

void C_Animation::GenerateBoneSegments(GameObject* bone)
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
		
		//display_bone.a = bone_transform->GetLocalPosition();
		//display_bone.b = bone->childs[i]->GetComponent<C_Transform>()->GetLocalPosition();

		display_bones.push_back(display_bone);

		GenerateBoneSegments(bone->childs[i]);
	}

	return;
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
	if (play && pause)
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

float C_Animation::GetTicksPerSecond() const
{
	return ((current_animation == nullptr) ? 0.0f : current_animation->GetTicksPerSecond());
}

float C_Animation::GetDuration() const
{
	return ((current_animation == nullptr) ? 0.0f : current_animation->GetDuration());
}

void C_Animation::SetPlaybackSpeed(const float& playback_speed)
{
	this->playback_speed = playback_speed;
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
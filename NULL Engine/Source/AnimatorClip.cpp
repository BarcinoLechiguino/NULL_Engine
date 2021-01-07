#include "R_Animation.h"

#include "AnimatorClip.h"

AnimatorClip::AnimatorClip() : 
animation			(nullptr), 
name				("[NONE]"), 
start				(0), 
end					(0), 
duration			(0.0f),
duration_in_seconds	(0.0f),
loop				(false),
time				(0.0f),
frame				(0.0f),
tick				(0)
{

}

AnimatorClip::AnimatorClip(const R_Animation* animation, const std::string& name, const uint& start, const uint& end, const bool& loop) : 
animation	(animation), 
name		(name), 
start		(start), 
end			(end),
loop		(loop),
time		(0.0f),
frame		(0.0f),
tick		(0)
{
	duration				= (float)(end - start);
	duration_in_seconds		= (animation != nullptr) ? (duration / animation->GetTicksPerSecond()) : 0.0f;
}

bool AnimatorClip::StepClip(const float& dt)
{
	if (animation == nullptr)
	{
		return false;
	}

	time	+= dt;
	frame	= time * animation->GetTicksPerSecond();
	tick	= (uint)floor(frame);

	if (time > duration_in_seconds)
	{
		ClearClip();
		return false;
	}

	return true;
}

void AnimatorClip::ClearClip()
{
	time	= 0.0f;
	frame	= 0.0f;
	tick	= 0;
}

// --- CLIP DEBUG METHODS
void AnimatorClip::StepClipToPrevKeyframe()
{
	tick = (tick != start) ? --tick : end;
}

void AnimatorClip::StepClipToNextKeyframe()
{
	tick = (tick != end) ? ++tick : start;
}

// --- CLIP TIME DATA GET/SET METHODS
void AnimatorClip::LoopClip(const bool& set_to)
{
	loop = set_to;
}

bool AnimatorClip::IsLooped() const
{
	return loop;
}

float AnimatorClip::GetClipTime() const
{
	return time;
}

float AnimatorClip::GetClipFrame() const
{
	return frame;
}

uint AnimatorClip::GetClipTick() const
{
	return tick;
}

float AnimatorClip::GetAnimationFrame() const
{
	return ((float)start + frame);
}

uint AnimatorClip::GetAnimationTick() const
{
	return (start + tick);
}

// --- GENERAL CLIP DATA GET METHODS
const R_Animation* AnimatorClip::GetAnimation() const
{
	return animation;
}

const char* AnimatorClip::GetAnimationName() const
{
	return ((animation != nullptr) ? animation->GetName() : "[NONE]");
}

float AnimatorClip::GetAnimationTicksPerSecond() const
{
	return ((animation != nullptr) ? animation->GetTicksPerSecond() : 0.0f);
}

float AnimatorClip::GetAnimationDuration() const
{
	return ((animation != nullptr) ? animation->GetDuration() : 0.0f);
}

const char* AnimatorClip::GetName() const
{
	return name.c_str();
}

uint AnimatorClip::GetStart() const
{
	return start;
}

uint AnimatorClip::GetEnd() const
{
	return end;
}

float AnimatorClip::GetDuration() const
{
	return duration;
}

float AnimatorClip::GetDurationInSeconds() const
{
	return duration_in_seconds;
}
#include "Channel.h"

Channel::Channel()
{
	name = "[NONE]";
}

Channel::Channel(const char* name)
{
	this->name = (name != nullptr) ? name : "[NONE]";
}

bool Channel::HasKeyframes(KEYFRAME_TYPE type) const
{
	switch (type)
	{
	case KEYFRAME_TYPE::POSITION:	{ return !(position_keyframes.size() == 1 && position_keyframes.begin()->first == -1); }	break;
	case KEYFRAME_TYPE::ROTATION:	{ return !(rotation_keyframes.size() == 1 && rotation_keyframes.begin()->first == -1); }	break;
	case KEYFRAME_TYPE::SCALE:		{ return !(scale_keyframes.size() == 1 && scale_keyframes.begin()->first == -1); }			break;
	}

	return false;
}

bool Channel::HasPositionKeyframes() const
{
	return HasKeyframes(KEYFRAME_TYPE::POSITION);
}

bool Channel::HasRotationKeyframes() const
{
	return HasKeyframes(KEYFRAME_TYPE::ROTATION);
}

bool Channel::HasScaleKeyframes() const
{
	return HasKeyframes(KEYFRAME_TYPE::SCALE);
}

PositionKeyframe Channel::GetPositionKeyframe(const double& current_keyframe) const
{
	PositionKeyframe item	= position_keyframes.find(current_keyframe);
	item					= (item == position_keyframes.end()) ? --item : item;

	return item;
}

RotationKeyframe Channel::GetRotationKeyframe(const double& current_keyframe) const
{
	RotationKeyframe item	= rotation_keyframes.find(current_keyframe);
	item					= (item == rotation_keyframes.end()) ? --item : item;

	return item;
}

ScaleKeyframe Channel::GetScaleKeyframe(const double& current_keyframe) const
{
	ScaleKeyframe item	= scale_keyframes.find(current_keyframe);
	item				= (item == scale_keyframes.end()) ? --item : item;

	return item;
}

PositionKeyframe Channel::GetPrevPositionKeyframe(const double& current_keyframe) const
{
	PositionKeyframe item	= GetPositionKeyframe(current_keyframe);
	item					= (item != position_keyframes.begin()) ? --item : item;

	return item;
}

RotationKeyframe Channel::GetPrevRotationKeyframe(const double& current_keyframe) const
{
	RotationKeyframe item	= GetRotationKeyframe(current_keyframe);
	item					= (item != rotation_keyframes.begin()) ? --item : item;

	return item;
}

ScaleKeyframe Channel::GetPrevScaleKeyframe(const double& current_keyframe) const
{
	ScaleKeyframe item	= GetScaleKeyframe(current_keyframe);
	item				= (item != scale_keyframes.begin()) ? --item : item;

	return item;
}

PositionKeyframe Channel::GetNextPositionKeyframe(const double& current_keyframe) const
{
	PositionKeyframe item	= GetPositionKeyframe(current_keyframe);
	item					= (item != position_keyframes.end()) ? ++item : item;

	return item;
}

RotationKeyframe Channel::GetNextRotationKeyframe(const double& current_keyframe) const
{
	RotationKeyframe item	= GetRotationKeyframe(current_keyframe);
	item					= (item != rotation_keyframes.end()) ? ++item : item;

	return item;
}

ScaleKeyframe Channel::GetNextScaleKeyframe(const double& current_keyframe) const
{
	ScaleKeyframe item	= GetScaleKeyframe(current_keyframe);
	item				= (item != scale_keyframes.end()) ? ++item : item;

	return item;
}

PositionKeyframe Channel::GetClosestPrevPositionKeyframe(const double& current_keyframe) const
{
	PositionKeyframe item	= position_keyframes.lower_bound(current_keyframe);
	item					= (item != position_keyframes.begin()) ? --item : item;

	return item;
}

RotationKeyframe Channel::GetClosestPrevRotationKeyframe(const double& current_keyframe) const
{
	RotationKeyframe item	= rotation_keyframes.lower_bound(current_keyframe);
	item					= (item != rotation_keyframes.begin()) ? --item : item;
	
	return item;
}

ScaleKeyframe Channel::GetClosestPrevScaleKeyframe(const double& current_keyframe) const
{
	ScaleKeyframe item	= scale_keyframes.lower_bound(current_keyframe);
	item				= (item != scale_keyframes.begin()) ? --item : item;

	return item;
}

PositionKeyframe Channel::GetClosestNextPositionKeyframe(const double& current_keyframe) const
{
	PositionKeyframe item	= position_keyframes.upper_bound(current_keyframe);
	item					= (item != position_keyframes.end()) ? item : --item;

	return item;
}

RotationKeyframe Channel::GetClosestNextRotationKeyframe(const double& current_keyframe) const
{
	RotationKeyframe item	= rotation_keyframes.upper_bound(current_keyframe);
	item					= (item != rotation_keyframes.end()) ? item : --item;

	return item;
}

ScaleKeyframe Channel::GetClosestNextScaleKeyframe(const double& current_keyframe) const
{
	ScaleKeyframe item	= scale_keyframes.upper_bound(current_keyframe);
	item				= (item != scale_keyframes.end()) ? item : --item;

	return item;
}
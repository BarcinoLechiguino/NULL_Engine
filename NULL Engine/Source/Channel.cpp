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
	return !(position_keyframes.size() == 1 && position_keyframes.begin()->first == -1);
}

bool Channel::HasRotationKeyframes() const
{
	return !(rotation_keyframes.size() == 1 && rotation_keyframes.begin()->first == -1);
}

bool Channel::HasScaleKeyframes() const
{
	return !(scale_keyframes.size() == 1 && scale_keyframes.begin()->first == -1);
}

PositionKeyframe Channel::GetPrevPositionKeyframe(double current_keyframe) const
{
	PositionKeyframe item	= position_keyframes.find(current_keyframe);
	item					= (item != position_keyframes.begin()) ? --item : item;

	return item;
}

RotationKeyframe Channel::GetPrevRotationKeyframe(double current_keyframe) const
{
	RotationKeyframe item	= rotation_keyframes.find(current_keyframe);
	item					= (item != rotation_keyframes.begin()) ? --item : item;

	return item;
}

ScaleKeyframe Channel::GetPrevScaleKeyframe(double current_keyframe) const
{
	ScaleKeyframe item	= scale_keyframes.find(current_keyframe);
	item				= (item != scale_keyframes.begin()) ? --item : item;

	return item;
}

PositionKeyframe Channel::GetNextPositionKeyframe(double current_keyframe) const
{
	PositionKeyframe item	= position_keyframes.find(current_keyframe);
	item					= (item != position_keyframes.end()) ? ++item : item;

	return item;
}

RotationKeyframe Channel::GetNextRotationKeyframe(double current_keyframe) const
{
	RotationKeyframe item	= rotation_keyframes.find(current_keyframe);
	item					= (item != rotation_keyframes.end()) ? ++item : item;

	return item;
}

ScaleKeyframe Channel::GetNextScaleKeyframe(double current_keyframe) const
{
	ScaleKeyframe item	= scale_keyframes.find(current_keyframe);
	item				= (item != scale_keyframes.end()) ? ++item : item;

	return item;
}

PositionKeyframe Channel::GetClosestPrevPositionKeyframe(double current_keyframe) const
{
	PositionKeyframe item	= position_keyframes.lower_bound(current_keyframe);
	item					= (item != position_keyframes.begin()) ? --item : item;

	return item;
}

RotationKeyframe Channel::GetClosestPrevRotationKeyframe(double current_keyframe) const
{
	RotationKeyframe item	= rotation_keyframes.lower_bound(current_keyframe);
	item					= (item != rotation_keyframes.begin()) ? --item : item;
	
	return item;
}

ScaleKeyframe Channel::GetClosestPrevScaleKeyframe(double current_keyframe) const
{
	ScaleKeyframe item	= scale_keyframes.lower_bound(current_keyframe);
	item				= (item != scale_keyframes.begin()) ? --item : item;

	return item;
}

PositionKeyframe Channel::GetClosestNextPositionKeyframe(double current_keyframe) const
{
	PositionKeyframe item	= position_keyframes.upper_bound(current_keyframe);
	item					= (item != position_keyframes.end()) ? item : --item;

	return item;
}

RotationKeyframe Channel::GetClosestNextRotationKeyframe(double current_keyframe) const
{
	RotationKeyframe item	= rotation_keyframes.upper_bound(current_keyframe);
	item					= (item != rotation_keyframes.end()) ? item : --item;

	return item;
}

ScaleKeyframe Channel::GetClosestNextScaleKeyframe(double current_keyframe) const
{
	ScaleKeyframe item	= scale_keyframes.upper_bound(current_keyframe);
	item				= (item != scale_keyframes.end()) ? item : --item;

	return item;
}
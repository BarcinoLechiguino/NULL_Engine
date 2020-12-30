#include "Channel.h"

template struct Keyframe<float3>;
template struct Keyframe<Quat>;

Channel::Channel()
{

}

Channel::Channel(const char* name)
{
	this->name = (name != nullptr) ? name : "[NONE]";
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

const Keyframe<float3> Channel::GetPrevPositionKeyframe(const double& current_keyframe) const
{
	std::map<double, float3>::const_iterator item = position_keyframes.find(current_keyframe);
	item = (item != position_keyframes.begin()) ? --item : item;

	return Keyframe<float3>(item->first, item->second);
}

const Keyframe<Quat> Channel::GetPrevRotationKeyframe(const double& current_keyframe) const
{
	std::map<double, Quat>::const_iterator item = rotation_keyframes.find(current_keyframe);
	item = (item != rotation_keyframes.begin()) ? --item : item;

	return Keyframe<Quat>(item->first, item->second);
}

const Keyframe<float3> Channel::GetPrevScaleKeyframe(const double& current_keyframe) const
{
	std::map<double, float3>::const_iterator item = scale_keyframes.find(current_keyframe);
	item = (item != scale_keyframes.begin()) ? --item : item;

	return Keyframe<float3>(item->first, item->second);
}

const Keyframe<float3> Channel::GetNextPositionKeyframe(const double& current_keyframe) const
{
	std::map<double, float3>::const_iterator item = position_keyframes.find(current_keyframe);
	item = (item != position_keyframes.begin()) ? ++item : item;

	return Keyframe<float3>(item->first, item->second);
}

const Keyframe<Quat> Channel::GetNextRotationKeyframe(const double& current_keyframe) const
{
	std::map<double, Quat>::const_iterator item = rotation_keyframes.find(current_keyframe);
	item = (item != rotation_keyframes.begin()) ? ++item : item;

	return Keyframe<Quat>(item->first, item->second);
}

const Keyframe<float3> Channel::GetNextScaleKeyframe(const double& current_keyframe) const
{
	std::map<double, float3>::const_iterator item = scale_keyframes.find(current_keyframe);
	item = (item != scale_keyframes.begin()) ? ++item : item;

	return Keyframe<float3>(item->first, item->second);
}

std::map<double, float3>::const_iterator Channel::GetPrevPositionKeyframe(double current_keyframe)
{
	std::map<double, float3>::const_iterator item = position_keyframes.find(current_keyframe);

	item = (item != position_keyframes.begin()) ? --item : item;
	//item = (item != position_keyframes.begin()) ? --item : position_keyframes.begin();

	return item;
}

std::map<double, Quat>::const_iterator Channel::GetPrevRotationKeyframe(double current_keyframe)
{
	std::map<double, Quat>::const_iterator item = rotation_keyframes.find(current_keyframe);

	item = (item != rotation_keyframes.begin()) ? --item : item;

	return item;
}

std::map<double, float3>::const_iterator Channel::GetPrevScaleKeyframe(double current_keyframe)
{
	std::map<double, float3>::const_iterator item = scale_keyframes.find(current_keyframe);

	item = (item != scale_keyframes.begin()) ? --item : item;

	return item;
}

std::map<double, float3>::const_iterator Channel::GetNextPositionKeyframe(double current_keyframe)
{
	std::map<double, float3>::const_iterator item = position_keyframes.find(current_keyframe);

	item = (item != position_keyframes.end()) ? ++item : item;

	return item;
}

std::map<double, Quat>::const_iterator Channel::GetNextRotationKeyframe(double current_keyframe)
{
	std::map<double, Quat>::const_iterator item = rotation_keyframes.find(current_keyframe);

	item = (item != rotation_keyframes.end()) ? ++item : item;

	return item;
}

std::map<double, float3>::const_iterator Channel::GetNextScaleKeyframe(double current_keyframe)
{
	std::map<double, float3>::const_iterator item = scale_keyframes.find(current_keyframe);

	item = (item != scale_keyframes.end()) ? ++item : item;

	return item;
}

// --- KEYFRAME METHODS
template <typename T>
Keyframe<T>::Keyframe() : time(-1.0f)
{

}

template <typename T>
Keyframe<T>::Keyframe(double time, T value) : time(time), value(value)
{

}
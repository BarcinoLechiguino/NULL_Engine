#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <map>

#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"

template <typename T>
struct Keyframe
{
	Keyframe();
	Keyframe(double time, T value);

	double time;
	T value;
};

struct Channel																						// Channels are the "Bones" of the animation. However, for distinction between this
{																									// "Bones" and the Mesh's bones they have been re-named to Channels.
	Channel();
	Channel(const char* name);

	bool HasPositionKeyframes() const;																// --- There will be no keyframes when:
	bool HasRotationKeyframes() const;																// Size == 1	(Initial Position. Always needed regardless)
	bool HasScaleKeyframes() const;																	// Time == -1	(Time cannot be negative, thus -1 is used as "non-valid" identifier)

	const Keyframe<float3>	GetPrevPositionKeyframe(const double& current_keyframe) const;
	const Keyframe<Quat>	GetPrevRotationKeyframe(const double& current_keyframe) const;
	const Keyframe<float3>	GetPrevScaleKeyframe(const double& current_keyframe) const;

	const Keyframe<float3>	GetNextPositionKeyframe(const double& current_keyframe) const;
	const Keyframe<Quat>	GetNextRotationKeyframe(const double& current_keyframe) const;
	const Keyframe<float3>	GetNextScaleKeyframe(const double& current_keyframe) const;

	std::map<double, float3>::const_iterator	GetPrevPositionKeyframe(double current_keyframe);	// 
	std::map<double, Quat>::const_iterator		GetPrevRotationKeyframe(double current_keyframe);	// 
	std::map<double, float3>::const_iterator	GetPrevScaleKeyframe(double current_keyframe);		// 

	std::map<double, float3>::const_iterator	GetNextPositionKeyframe(double current_keyframe);	// 
	std::map<double, Quat>::const_iterator		GetNextRotationKeyframe(double current_keyframe);	// 
	std::map<double, float3>::const_iterator	GetNextScaleKeyframe(double current_keyframe);		// 

	std::map<double, float3>	position_keyframes;													// Position-related keyframes.
	std::map<double, Quat>		rotation_keyframes;													// Rotation-related keyframes.
	std::map<double, float3>	scale_keyframes;													// Scale-related keyframes.

	std::string					name;																// Name of the Channel/Bone
};

#endif // !__CHANNEL_H__
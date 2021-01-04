#ifndef __KEYFRAME_H__
#define __KEYFRAME_H__

template <typename T>
struct Keyframe
{
	Keyframe();
	Keyframe(double time, T value);

	double time;
	T value;
};

#endif // !__KEYFRAME_H__

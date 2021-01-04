#include "MathGeoTransform.h"

#include "Keyframe.h"

template struct Keyframe<float3>;
template struct Keyframe<Quat>;

template <typename T>
Keyframe<T>::Keyframe() : time(-1.0f)
{

}

template <typename T>
Keyframe<T>::Keyframe(double time, T value) : time(time), value(value)
{

}
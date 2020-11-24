#ifndef __RANDOM_H__
#define __RANDOM_H__

#include "VarTypedefs.h"

namespace Random
{
	namespace LCG													// RNG library included with MathGeoLib. Average Performance (Real random factor...)
	{
		uint GetRandomUint(uint min = 0, uint max = 2147483647);	// 
		float GetRandomFloat(float min, float max);					// 
	}

	namespace PCG													// RNG library. Great Performance (Real random factor...)
	{
		uint GetRandomUint(uint min = 0, uint max = 2147483647);	// 
		float GetRandomFloat(float min, float max);					// 
	}
}

#endif // !__RANDOM_H__
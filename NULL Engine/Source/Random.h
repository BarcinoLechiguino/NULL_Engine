#ifndef __RANDOM_H__
#define __RANDOM_H__

#include "MathGeoLib/include/Algorithm/Random/LCG.h"
#include "VarTypedefs.h"

namespace Random
{
	namespace LCG																// RNG library included with MathGeoLib. Average Performance (Real random factor...)
	{	
		static math::LCG lcg_rand;

		uint	GetRandomUint();												// 
		uint	GetBoundedRandomUint(uint min = 0, uint max = 2147483647);		// 
		float	GetBoundedRandomFloat(float min, float max);					// 
	}

	namespace PCG																// RNG library. Great Performance (Real random factor...)
	{
		void	InitPCG();
		
		uint	GetRandomUint();												// 
		uint	GetBoundedRandomUint(uint max = 2147483647);					// 
		float	GetBoundedRandomFloat(float max);								// 
	}
}

#endif // !__RANDOM_H__
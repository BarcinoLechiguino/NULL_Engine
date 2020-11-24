#include "MathStructures.h"
#include "MathGeoLib/include/Algorithm/Random/LCG.h"
#include "PCG/include/pcg_basic.h"

#include "Random.h"

uint Random::LCG::GetRandomUint(uint min, uint max)
{
	math::LCG lcg_rand;
	
	return (uint)lcg_rand.Int(min, max);
}

float Random::LCG::GetRandomFloat(float min, float max)
{
	math::LCG lcg_rand;
	
	return lcg_rand.Float(min, max);
}

uint Random::PCG::GetRandomUint(uint min, uint max)
{
	return pcg32_boundedrand(max);
}

float Random::PCG::GetRandomFloat(float min, float max)
{
	return 0.0f;
}
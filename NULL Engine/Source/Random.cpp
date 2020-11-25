#include "MathStructures.h"

#include "PCG/include/pcg_basic.h"

#include "Random.h"

// --- LCG ---
uint Random::LCG::GetRandomUint()
{	
	return (uint)lcg_rand.Int();
}

uint Random::LCG::GetBoundedRandomUint(uint min, uint max)
{
	return (uint)lcg_rand.Int(min, max);
}

float Random::LCG::GetBoundedRandomFloat(float min, float max)
{	
	return lcg_rand.Float(min, max);
}

// --- PCG ---
void Random::PCG::InitPCG()
{
	pcg32_srandom(42u, 54u);
}

uint Random::PCG::GetRandomUint()
{
	return pcg32_random();
}

uint Random::PCG::GetBoundedRandomUint(uint max)
{
	/*pcg32_random_t rng;
	return pcg32_boundedrand_r(&rng, max);*/
	
	return pcg32_boundedrand(max);
}

float Random::PCG::GetBoundedRandomFloat(float max)
{
	return 0.0f;
}
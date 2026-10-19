#include "EABRandom.h"

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <chrono>
#include <iostream>
#include <random>
#include <stdexcept>

// Random number generation for the EAB model, using the GNU Scientific Library.
// Replaces the NAG G05 routines.

namespace
{
	gsl_rng* EABrng = nullptr;
	unsigned long EABseed = 0;

	gsl_rng* Generator()
	{
		if (EABrng == nullptr)
			RNG_Init(0); 
		return EABrng;
	}
}

void RNG_Init(unsigned long seed)
{
	if (EABrng == nullptr)
		EABrng = gsl_rng_alloc(gsl_rng_mt19937);
	if (EABrng == nullptr)
		throw std::logic_error("Could not allocate GSL random number generator");

	if (seed == 0)
	{
		std::random_device rd;
		seed = (static_cast<unsigned long>(rd()) << 16) ^
			static_cast<unsigned long>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		seed &= 0xFFFFFFFFUL; 
		if (seed == 0)
			seed = 4357;
	}
	EABseed = seed;
	gsl_rng_set(EABrng, EABseed);
	std::cout << "Random seed: " << EABseed << std::endl;
}

void RNG_Free()
{
	if (EABrng != nullptr)
	{
		gsl_rng_free(EABrng);
		EABrng = nullptr;
	}
}

unsigned long RNG_GetSeed()
{
	return EABseed;
}

double RNG_Uniform()
{
	return gsl_rng_uniform_pos(Generator());
}

unsigned int RNG_Poisson(double mean)
{
	if (mean <= 0.0)
		return 0;
	return gsl_ran_poisson(Generator(), mean);
}

int RNG_Binomial(int n, double p)
{
	if (n <= 0 || p <= 0.0)
		return 0;
	if (p >= 1.0)
		return n;
	return static_cast<int>(gsl_ran_binomial(Generator(), p, static_cast<unsigned int>(n)));
}

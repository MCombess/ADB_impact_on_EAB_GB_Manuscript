#ifndef __EABRandom_H__
#define __EABRandom_H__

// Random number generation for the EAB model, using the GNU Scientific Library.
// Replaces the NAG G05 routines.

void RNG_Init(unsigned long seed = 0);
void RNG_Free();
unsigned long RNG_GetSeed();

double RNG_Uniform();                          
unsigned int RNG_Poisson(double mean);             
int RNG_Binomial(int n, double p);        

#endif

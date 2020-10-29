#include "Random.h"
#include <stdexcept>
#include <chrono>

//Random constants
#define RANDOM_MUL	0x5DEECE66DLL
#define RANDOM_ADD	0xBLL
#define RANDOM_AND	((1LL << 48) - 1)

//Random seeds
int64_t RandomTimeSeed()
{
	auto now = std::chrono::high_resolution_clock::now();
	auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
	return (int64_t)nanos;
}

//Constructors
Random::Random()
{
	return;
}

Random::Random(int64_t set_seed)
{
	//Use given seed
	SetSeed(set_seed);
}

//Get and set seed
int64_t Random::GetSeed() const
{
	//Return seed
	return seed;
}

void Random::SetSeed(int64_t set_seed)
{
	//Scramble seed a little bit
	seed = (set_seed ^ RANDOM_MUL) & RANDOM_AND;
}

//Next seed
int32_t Random::Next(int32_t bits)
{
	seed = (seed * RANDOM_MUL + RANDOM_ADD) & RANDOM_AND;
	return (int32_t)((uint64_t)seed >> (48 - bits));
}

//Next values
bool Random::NextBoolean()
{
	//Return random bit as boolean
	return Next(1) == 1;
}

int32_t Random::NextInt()
{
	//Return random 32 bit integer
	return Next(32);
}

int32_t Random::NextInt(int32_t bound)
{
	//Verify that our bound is positive and non-zero
	if (bound <= 0)
		throw std::invalid_argument("bound must be positive");
	
	//If bound is maskable, return mask result
	if((bound & -bound) == bound)
		return (int32_t)((bound * (int64_t)Next(31)) >> 31);
	
	//Some weird modulo crap if bound isn't maskable
	int32_t bits, value;
	do
	{
		bits = Next(31);
		value = bits % bound;
	} while (bits - value + (bound - 1) < 0);
	return value;
}

float Random::NextFloat()
{
	//Return random float (24-bit integer divided by 24 bit limit)
	return Next(24) / ((float)(1LL << 24));
}

double Random::NextDouble()
{
	//Return random double ((supposed to be) 54-bit integer divided by 54 bit limit)
	return (((int64_t)Next(27) << 27) + Next(27)) / (double)(1LL << 54);
}

int64_t Random::NextLong()
{
	//Return random 64 bit integer (two 32 bit integers put together)
	return ((int64_t)(Next(32)) << 32) + Next(32);
}

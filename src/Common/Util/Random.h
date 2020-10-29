#pragma once
#include <cstdint>

//Random seeds
int64_t RandomTimeSeed();

//Random class
class Random
{
	private:
		int64_t seed;
		
	public:
		//Constructors
		Random();
		Random(int64_t set_seed);
		
		//Get and set seed
		int64_t GetSeed() const;
		void SetSeed(int64_t set_seed);
		
		//Next seed
		int32_t Next(int32_t bits);
		
		//Next values
		bool NextBoolean();
		
		int32_t NextInt();
		int32_t NextInt(int32_t bound);
		
		int64_t NextLong();
		
		float NextFloat();
		double NextDouble();
};

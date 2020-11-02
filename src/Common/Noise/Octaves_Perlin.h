#pragma once
#include <cstdint>
#include <Common/Util/Random.h>
#include "Perlin.h"

namespace Noise
{
	class Octaves_Perlin
	{
		private:
			//Perlin octaves
			Perlin **octave = nullptr;
			int32_t octaves = 0;
			
		public:
			//Constructor and destructor
			Octaves_Perlin();
			Octaves_Perlin(Random &random, int32_t set_octaves);
			Octaves_Perlin(int64_t seed, int32_t set_octaves);
			~Octaves_Perlin();
			
			//Octaves perlin interface
			bool SetOctaves(Random &random, int32_t set_octaves);
			bool SetOctaves(int64_t seed, int32_t set_octaves);
			
			//2D point noise
			double Noise(const double x, const double y);
			
			//3D area noise
			void Noise(double out[], const double xin, const double yin, const double zin, const int32_t xl, const int32_t yl, const int32_t zl, double xm, double ym, double zm);
			
			//2D area noise
			void Noise(double out[], const double xin, const double zin, const int32_t xl, const int32_t zl, const double xm, const double zm);
	};
}

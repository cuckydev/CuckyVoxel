#pragma once
#include <cmath>
#include <cstdint>
#include <Common/Util/Random.h>

namespace Noise
{
	//2D Simplex Noise class
	class Simplex
	{
		private:
			//Permutation array
			int32_t permutations[512];
			
		public:
			//Random coordinate offset
			double x_coord;
			double y_coord;
			
		private:
			//Initialization method
			void InitCoordAndPermutations(Random &random);
			
			//Small utility methods
			inline int32_t fastfloor(const double x)
			{ return (x > 0.0) ? ((int32_t)x) : ((int32_t)x - 1); }
			
			inline double dot(const int32_t g[], const double x, const double y)
			{ return g[0] * x + g[1] * y; }
			
		public:
			//Constructors
			Simplex();
			Simplex(Random &random);
			
			//Noise method (http://staffwww.itn.liu.se/~stegu/simplexnoise/SimplexNoise.java)
			void Noise(double out[], const double xin, const double yin, const int32_t xl, const int32_t yl, const double xm, const double ym, const double out_mult);
	};
}

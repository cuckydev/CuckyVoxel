#pragma once
#include <cstdint>
#include <Common/Util/Random.h>

namespace Noise
{
	//3D Perlin Noise class
	class Perlin
	{
		private:
			//Permutation array
			int32_t permutations[512];
			
		public:
			//Random coordinate offset
			double x_coord;
			double y_coord;
			double z_coord;
			
		private:
			//Initialization method
			void InitCoordAndPermutations(Random &random);
			
			//Small utility methods
			inline int32_t fastfloor(const double x)
			{ return (x > 0.0) ? ((int32_t)x) : ((int32_t)x - 1); }
			
			inline double fade(double t)
			{ return t * t * t * (t * (t * 6.0 - 15.0) + 10.0); }
			
			inline double lerp(double t, double a, double b)
			{ return a + t * (b - a); }
			
			inline double grad2d(int32_t hash, double x, double y)
			{
				//Convert low 4 bits of hash code into 12 gradient directions
				int32_t h = hash & 0xF;
				double	u = (1 - ((h & 0x8) >> 3)) * x,
						v = (h < 4) ? 0.0 : ((h == 12 || h == 14) ? x : y);
				return (((h & 1) == 0) ? u : -u) + (((h & 2) == 0) ? v : -v);
			}
			
			inline double grad3d(int32_t hash, double x, double y, double z)
			{
				//Convert low 4 bits of hash code into 12 gradient directions
				int32_t h = hash & 0xF;
				double	u = (h < 8) ? x : y,
						v = (h < 4) ? y : ((h == 12 || h == 14) ? x : z);
				return (((h & 1) == 0) ? u : -u) + (((h & 2) == 0) ? v : -v);
			}
			
		public:
			//Constructors
			Perlin();
			Perlin(Random &random);
			
			//Noise method (https://mrl.nyu.edu/~perlin/noise/)
			double Noise(const double xin, const double yin, const double zin);
			
			//2D noise overload
			double Noise(const double xin, const double yin);
			
			//Area noise overload (potentially http://staffwww.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf)
			void Noise(double out[], const double xin, const double yin, const double zin, const int32_t xl, const int32_t yl, const int32_t zl, const double xm, const double ym, const double zm, const double out_div);
	};
}

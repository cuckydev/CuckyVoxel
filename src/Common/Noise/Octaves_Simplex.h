#pragma once
#include <cstdint>
#include <Common/Util/Random.h>
#include "Simplex.h"

namespace Noise
{
	class Octaves_Simplex
	{
		private:
			//Simplex octaves
			Simplex *octave;
			int32_t octaves;
			
		public:
			//Constructor and destructor
			Octaves_Simplex();
			Octaves_Simplex(Random &random, int32_t set_octaves);
			~Octaves_Simplex();
			
			//Octaves simplex interface
			bool SetOctaves(Random &random, int32_t set_octaves);
			
			//2D area noise, out_mult set to 0.5
			void Noise(double out[], const double xin, const double yin, const int32_t xl, const int32_t yl, const double xm, const double ym, const double move_mult);
			
			//2D area noise
			void Noise(double out[], const double xin, const double yin, const int32_t xl, const int32_t yl, double xm, double ym, const double move_mult, const double out_mult);
	};
}

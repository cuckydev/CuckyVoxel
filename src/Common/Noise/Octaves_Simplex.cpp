#include "Octaves_Simplex.h"

namespace Noise
{
	//Constructor and destructor
	Octaves_Simplex::Octaves_Simplex()
	{
		return;
	}
	
	Octaves_Simplex::Octaves_Simplex(Random &random, int32_t set_octaves)
	{
		//Set octaves
		SetOctaves(random, set_octaves);
	}
	
	Octaves_Simplex::~Octaves_Simplex()
	{
		//Delete octaves
		delete[] octave;
	}
	
	//Octaves simplex interface
	bool Octaves_Simplex::SetOctaves(Random &random, int32_t set_octaves)
	{
		//Set octaves
		octaves = set_octaves;
		
		delete[] octave;
		octave = new Simplex[set_octaves]{random};
		
		return octave != nullptr;
	}
	
	//2D area noise, outMult set to 0.5
	void Octaves_Simplex::Noise(double out[], const double xin, const double yin, const int32_t xl, const int32_t yl, const double xm, const double ym, const double move_mult)
	{
		Noise(out, xin, yin, xl, yl, xm, ym, move_mult, 0.5);
	}
	
	//2D area noise
	void Octaves_Simplex::Noise(double out[], const double xin, const double yin, const int32_t xl, const int32_t yl, double xm, double ym, const double move_mult, const double out_mult)
	{
		xm /= 1.5;
		ym /= 1.5;
		
		double move_factor = 1.0;
		double out_factor = 1.0;
		for (int32_t j = 0; j < octaves; j++)
		{
			octave[j].Noise(out, xin, yin, xl, yl, xm * move_factor, ym * move_factor, 0.55 / out_factor);
			move_factor *= move_mult;
			out_factor *= out_mult;
		}
	}
}

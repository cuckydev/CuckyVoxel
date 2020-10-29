#include "Octaves_Perlin.h"

namespace Noise
{
	//Constructor and destructor
	Octaves_Perlin::Octaves_Perlin()
	{
		return;
	}
	
	Octaves_Perlin::Octaves_Perlin(Random &random, int32_t set_octaves)
	{
		//Set octaves
		SetOctaves(random, set_octaves);
	}
	
	Octaves_Perlin::~Octaves_Perlin()
	{
		//Delete octaves
		delete[] octave;
	}
	
	//Octaves perlin interface
	bool Octaves_Perlin::SetOctaves(Random &random, int32_t set_octaves)
	{
		//Set octaves
		octaves = set_octaves;
		
		delete[] octave;
		octave = new Perlin[set_octaves]{random};
		
		return octave != nullptr;
	}
	
	//2D point noise
	double Octaves_Perlin::Noise(const double x, const double y)
	{
		double total = 0.0;
		double factor = 1.0;
		for (int32_t i = 0; i < octaves; i++)
		{
			total += octave[i].Noise(x * factor, y * factor) / factor;
			factor /= 2.0;
		}
		return total;
	}
	
	//3D area noise
	void Octaves_Perlin::Noise(double out[], const double xin, const double yin, const double zin, const int32_t xl, const int32_t yl, const int32_t zl, double xm, double ym, double zm)
	{
		double factor = 1.0;
		for (int32_t j = 0; j < octaves; j++)
		{
			octave[j].Noise(out, xin, yin, zin, xl, yl, zl, xm * factor, ym * factor, zm * factor, factor);
			factor /= 2.0;
		}
	}
	
	//2D area noise
	void Octaves_Perlin::Noise(double out[], const double xin, const double zin, const int32_t xl, const int32_t zl, const double xm, const double zm)
	{
		Noise(out, xin, 10.0, zin, xl, 1, zl, xm, 1.0, zm);
	}
}

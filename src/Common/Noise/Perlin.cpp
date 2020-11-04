#include "Perlin.h"

//#define FIX_OVERFLOW_BUGS //Fix overflow bugs (i.e far lands)

#include <cmath>

namespace Noise
{
	//Initialization method
	void Perlin::InitCoordAndPermutations(Random &random)
	{
		//Initialize coords
		x_coord = random.NextDouble() * 256.0;
		y_coord = random.NextDouble() * 256.0;
		z_coord = random.NextDouble() * 256.0;
		
		//Initialize permutation array
		for (int32_t i = 0; i < 256; i++)
			permutations[i] = i;
		
		for (int32_t i = 0; i < 256; i++)
		{
			const int32_t n = random.NextInt(256 - i) + i;
			const int32_t n2 = permutations[i];
			permutations[i] = permutations[n];
			permutations[n] = n2;
			permutations[i + 256] = permutations[i];
		}
	}
	
	//Constructors
	Perlin::Perlin()
	{
		//Call initialization with a new random
		Random temp_random(RandomTimeSeed());
		InitCoordAndPermutations(temp_random);
	}
	
	Perlin::Perlin(Random &random)
	{
		//Call initialization with given random
		InitCoordAndPermutations(random);
	}
	
	//Noise method (https://mrl.nyu.edu/~perlin/noise/)
	double Perlin::Noise(const double xin, const double yin, const double zin)
	{
		//Get our coordinate with our given offsets
		#ifdef FIX_OVERFLOW_BUGS
			double x = std::fmod(xin, 256.0) + x_coord;
			double y = std::fmod(yin, 256.0) + y_coord;
			double z = std::fmod(zin, 256.0) + z_coord;
		#else
			double x = xin + x_coord;
			double y = yin + y_coord;
			double z = zin + z_coord;
		#endif
		
		//Find unit cube that contains point
		const int32_t X = fastfloor(x) & 0xFF;
		const int32_t Y = fastfloor(y) & 0xFF;
		const int32_t Z = fastfloor(z) & 0xFF;
		
		//Find relative point of point in cube (X,Y,Z)
		x -= fastfloor(x);
		y -= fastfloor(y);
		z -= fastfloor(z);
		
		//Compute fade curves for each of X,Y,Z
		const double u = fade(x);
		const double v = fade(y);
		const double w = fade(z);
		
		//Hash coordinates of the 8 cube corners
		const int32_t A = permutations[X    ] + Y, AA = permutations[A] + Z, AB = permutations[A + 1] + Z,      
		              B = permutations[X + 1] + Y, BA = permutations[B] + Z, BB = permutations[B + 1] + Z;
		
		//And add blended results from 8 corners of cube
		return lerp(w, lerp(v, lerp(u, grad3d(permutations[AA    ], x    , y    , z     ),
		                               grad3d(permutations[BA    ], x - 1, y    , z     )),
		                       lerp(u, grad3d(permutations[AB    ], x    , y - 1, z     ),
		                               grad3d(permutations[BB    ], x - 1, y - 1, z     ))),
		               lerp(v, lerp(u, grad3d(permutations[AA + 1], x    , y    , z - 1 ),
		                               grad3d(permutations[BA + 1], x - 1, y    , z - 1 )),
		                       lerp(u, grad3d(permutations[AB + 1], x    , y - 1, z - 1 ),
		                               grad3d(permutations[BB + 1], x - 1, y - 1, z - 1 ))));
	}
	
	//2D noise overload
	double Perlin::Noise(const double xin, const double yin)
	{
		return Noise(xin, yin, 0.0);
	}
	
	//Area noise overload (potentially http://staffwww.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf)
	void Perlin::Noise(double out[], const double xin, const double yin, const double zin, const int32_t xl, const int32_t yl, const int32_t zl, const double xm, const double ym, const double zm, const double out_div)
	{
		#ifdef FIX_OVERFLOW_BUGS
			//Get bounds of the noise area before it repeats
			const double xmm = 256.0 / xm;
			const double ymm = 256.0 / ym;
			const double zmm = 256.0 / zm;
		#endif
		
		double *outp = out;
		if (yl == 1)
		{
			//2D noise
			for (int32_t xi = 0; xi < xl; xi++)
			{
				#ifdef FIX_OVERFLOW_BUGS
					double x = (std::fmod(xin, xmm) + xi) * xm + x_coord;
				#else
					double x = (xin + xi) * xm + x_coord;
				#endif
				
				int32_t X = fastfloor(x); //Find unit grid cell containing X
				x -= X; //Get relative X in cell
				X &= 0xFF; //Wrap integer cell at 255
				const double u = fade(x); //Compute the fade curve value of X
				
				for (int32_t zi = 0; zi < zl; zi++)
				{
					#ifdef FIX_OVERFLOW_BUGS
						double z = (std::fmod(zin, zmm) + zi) * zm + z_coord;
					#else
						double z = (zin + zi) * zm + z_coord;
					#endif
					
					int32_t Z = fastfloor(z); //Find unit grid cell containing Z
					z -= Z; //Get relative Z in cell
					Z &= 0xFF; //Wrap integer cell at 255
					const double w = fade(z); //Compute the fade curve value of Z
					
					const int32_t n8 = permutations[X] + 0;
					const int32_t n9 = permutations[n8] + Z;
					const int32_t n10 = permutations[permutations[X + 1] + 0] + Z;
					
					const double nxz0 = lerp(u, grad2d(permutations[n9    ], x, z),            grad3d(permutations[n10    ], x - 1.0, 0.0, z));
					const double nxz1 = lerp(u, grad3d(permutations[n9 + 1], x, 0.0, z - 1.0), grad3d(permutations[n10 + 1], x - 1.0, 0.0, z - 1.0));
					const double nxz = lerp(w, nxz0, nxz1);
					
					*outp++ += nxz / out_div;
				}
			}
		}
		else
		{
			//3D noise
			int32_t n10 = -1;
			double lerp6 = 0.0;
			double lerp3 = 0.0;
			double lerp7 = 0.0;
			double lerp4 = 0.0;
			
			for (int32_t xi = 0; xi < xl; xi++)
			{
				#ifdef FIX_OVERFLOW_BUGS
					double x = (std::fmod(xin, xmm) + xi) * xm + x_coord;
				#else
					double x = (xin + xi) * xm + x_coord;
				#endif
				
				int32_t X = fastfloor(x); //Find unit grid cell containing X
				x -= X; //Get relative X in cell
				X &= 0xFF; //Wrap integer cell at 255
				const double u = fade(x); //Compute the fade curve value of X
				
				for (int32_t zi = 0; zi < zl; zi++)
				{
					#ifdef FIX_OVERFLOW_BUGS
						double z = (std::fmod(zin, zmm) + zi) * zm + z_coord;
					#else
						double z = (zin + zi) * zm + z_coord;
					#endif
					
					int32_t Z = fastfloor(z); //Find unit grid cell containing Z
					z -= Z; //Get relative Z in cell
					Z &= 0xFF; //Wrap integer cell at 255
					const double v = fade(z); //Compute the fade curve value of Z
					
					for (int32_t yi = 0; yi < yl; yi++)
					{
						#ifdef FIX_OVERFLOW_BUGS
							double y = (std::fmod(yin, ymm) + yi) * ym + y_coord;
						#else
							double y = (yin + yi) * ym + y_coord;
						#endif
						
						int32_t Y = fastfloor(y); //Find unit grid cell containing Y
						y -= Y; //Get relative Y in cell
						Y &= 0xFF; //Wrap integer cell at 255
						const double w = fade(y); //Compute the fade curve value of Y
						
						if (yi == 0 || Y != n10)
						{
							n10 = Y;
							
							const int32_t n20 = permutations[X] + Y;
							const int32_t n21 = permutations[n20] + Z;
							const int32_t n22 = permutations[n20 + 1] + Z;
							
							const int32_t n23 = permutations[X + 1] + Y;
							const int32_t n24 = permutations[n23] + Z;
							const int32_t n25 = permutations[n23 + 1] + Z;
							
							lerp6 = lerp(u, grad3d(permutations[n21    ], x, y,       z),       grad3d(permutations[n24    ], x - 1.0, y,       z));
							lerp3 = lerp(u, grad3d(permutations[n22    ], x, y - 1.0, z),       grad3d(permutations[n25    ], x - 1.0, y - 1.0, z));
							lerp7 = lerp(u, grad3d(permutations[n21 + 1], x, y,       z - 1.0), grad3d(permutations[n24 + 1], x - 1.0, y,       z - 1.0));
							lerp4 = lerp(u, grad3d(permutations[n22 + 1], x, y - 1.0, z - 1.0), grad3d(permutations[n25 + 1], x - 1.0, y - 1.0, z - 1.0));
						}
						
						const double lerp8 = lerp(v, lerp(w, lerp6, lerp3), lerp(w, lerp7, lerp4));
						*outp++ += lerp8 / out_div;
					}
				}
			}
		}
	}
}

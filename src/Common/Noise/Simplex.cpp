#include "Simplex.h"

namespace Noise
{
	//Simplex constants
	static const int32_t grad[12][3] = {{1, 1, 0}, {-1, 1, 0}, {1, -1, 0}, {-1, -1, 0}, {1, 0, 1}, {-1, 0, 1}, {1, 0, -1}, {-1, 0, -1}, {0, 1, 1}, {0, -1, 1}, {0, 1, -1}, {0, -1, -1}};
	static const double F2 = 0.5 * (std::sqrt(3.0) - 1.0);
	static const double G2 = (3.0 - std::sqrt(3.0)) / 6.0;
	
	//Initialization method
	void Simplex::InitCoordAndPermutations(Random &random)
	{
		//Initialize coords
		x_coord = random.NextDouble() * 256.0;
		y_coord = random.NextDouble() * 256.0;
		random.NextDouble(); //z_coord
		
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
	Simplex::Simplex()
	{
		//Call initialization with a new random
		Random tempRandom(RandomTimeSeed());
		InitCoordAndPermutations(tempRandom);
	}
	
	Simplex::Simplex(Random &random)
	{
		//Call initialization with given random
		InitCoordAndPermutations(random);
	}
	
	//Noise method (http://staffwww.itn.liu.se/~stegu/simplexnoise/SimplexNoise.java)
	void Simplex::Noise(double out[], const double xin, const double yin, const int32_t xl, const int32_t yl, const double xm, const double ym, const double out_mult)
	{
		double *outp = out;
		for (int32_t xi = 0; xi < xl; xi++)
		{
			const double nx = (xin + xi) * xm + x_coord;
			for (int32_t yi = 0; yi < yl; yi++)
			{
				const double ny = (yin + yi) * ym + y_coord;
				
				//Skew the input space to determine which simplex cell we're in
				const double s = (nx + ny) * F2; //Hairy factor for 2D
				const int32_t i = fastfloor(nx + s);
				const int32_t j = fastfloor(ny + s);
				const double t = (i + j) * G2;
				const double X0 = i - t; //Unskew the cell origin back to (x,y) space
				const double Y0 = j - t;
				const double x0 = nx - X0; //The x,y distances from the cell origin
				const double y0 = ny - Y0;
				
				//For the 2D case, the simplex shape is an equilateral triangle.
				//Determine which simplex we are in.
				int32_t i1, j1; //Offsets for second (middle) corner of simplex in (i,j) coords
				if (x0 > y0)
				{
					i1 = 1;
					j1 = 0;
				}
				else
				{
					i1 = 0;
					j1 = 1;
				}
				
				//A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
				//a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
				//c = (3-sqrt(3))/6
				const double x1 = x0 - i1 + G2; //Offsets for middle corner in (x,y) unskewed coords
				const double y1 = y0 - j1 + G2;
				const double x2 = x0 - 1.0 + 2.0 * G2; //Offsets for last corner in (x,y) unskewed coords
				const double y2 = y0 - 1.0 + 2.0 * G2;
				
				//Work out the hashed gradient indices of the three simplex corners
				const int32_t ii = i & 0xFF;
				const int32_t jj = j & 0xFF;
				const int32_t gi0 = permutations[ii +      permutations[jj]]      % 12;
				const int32_t gi1 = permutations[ii + i1 + permutations[jj + j1]] % 12;
				const int32_t gi2 = permutations[ii + 1 +  permutations[jj + 1]]  % 12;
				
				//Calculate the contribution from the three corners
				double n0, n1, n2; //Noise contributions from the three corners
				
				double t0 = 0.5 - x0 * x0 - y0 * y0;
				if (t0 < 0.0)
				{
					n0 = 0.0;
				}
				else
				{
					t0 *= t0;
					n0 = t0 * t0 * dot(grad[gi0], x0, y0); //(x,y) of grad3 used for 2D gradient
				}
				
				double t1 = 0.5 - x1 * x1 - y1 * y1;
				if (t1 < 0.0)
				{
					n1 = 0.0;
				}
				else
				{
					t1 *= t1;
					n1 = t1 * t1 * dot(grad[gi1], x1, y1);
				}
				
				double t2 = 0.5 - x2 * x2 - y2 * y2;
				if (t2 < 0.0)
				{
					n2 = 0.0;
				}
				else
				{
					t2 *= t2;
					n2 = t2 * t2 * dot(grad[gi2], x2, y2);
				}
				
				//Add contributions from each corner to get the final noise value.
				//The result is scaled to return values in the interval [-1,1].
				*outp++ += 70.0 * (n0 + n1 + n2) * out_mult;
			}
		}
	}
}

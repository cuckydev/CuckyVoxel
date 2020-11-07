#include "ColourSpace.h"

#include <cmath>

namespace ColourSpace
{
	RGB HSVToRGB(const HSV &hsv)
	{
		float H = hsv.h, S = hsv.s, V = hsv.v;
		
		H *= 6.0f;
		float fract = H - std::floor(H);
		
		float P = V * (1.0f - S);
		float Q = V * (1.0f - S * fract);
		float T = V * (1.0f - S * (1.0f - fract));
		
		if      (0.0f <= H && H < 1.0f)
			return {V, T, P};
		else if (1.0f <= H && H < 2.0f)
			return {Q, V, P};
		else if (2.0f <= H && H < 3.0f)
			return {P, V, T};
		else if (3.0f <= H && H < 4.0f)
			return {P, Q, V};
		else if (4.0f <= H && H < 5.0f)
			return {T, P, V};
		else if (5.0f <= H && H < 6.0f)
			return {V, P, Q};
		else
			return {0.0f, 0.0f, 0.0f};
	}
}

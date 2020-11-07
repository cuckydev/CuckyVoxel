#pragma once

namespace ColourSpace
{
	struct RGB
	{
		float r, g, b;
	};
	
	struct HSV
	{
		float h, s, v;
	};
	
	RGB HSVToRGB(const HSV &HSV);
}

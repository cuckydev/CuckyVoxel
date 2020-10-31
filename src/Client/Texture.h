#pragma once
#include <string>
#include <istream>
#include <cstdint>

#include <Common/Util/Error.h>

namespace Texture
{
	//Texture types
	struct Data
	{
		Error error;
		uint8_t *data = nullptr;
		unsigned int width, height;
	};
	
	//Texture interface
	bool ReadTexture(Data &tex_data, std::string path);
	bool ReadTexture(Data &tex_data, std::istream &stream);
}

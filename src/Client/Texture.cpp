#include "Texture.h"
#include <fstream>
#include <cstddef>

#include <lodepng/lodepng.h>

namespace Texture
{
	//Texture interface
	bool ReadTexture(Data &tex_data, std::string path)
	{
		//Read from path as a file
		std::ifstream stream(path, std::ifstream::binary);
		if (!stream)
		{
			tex_data.error.Push("Failed to open texture file at " + path);
			return true;
		}
		return ReadTexture(tex_data, stream);
	}
	
	bool ReadTexture(Data &tex_data, std::istream &stream)
	{
		//Get length of stream
		stream.seekg(0, stream.end);
		size_t size = stream.tellg();
		stream.seekg(0, stream.beg);
		
		//Read data
		char *buffer = new char[size];
		if (buffer == nullptr)
		{
			tex_data.error.Push("Failed to allocate stream buffer");
			return true;
		}
		stream.read(buffer, size);
		
		//Process data as PNG
		unsigned int decode_error;
		if ((decode_error = lodepng_decode32((unsigned char**)&tex_data.data, &tex_data.width, &tex_data.height, (const unsigned char*)buffer, size)) != 0)
		{
			tex_data.error.Push(lodepng_error_text(decode_error));
			return true;
		}
		
		return false;
	}
}

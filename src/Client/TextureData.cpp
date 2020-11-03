#include "TextureData.h"
#include <fstream>
#include <cstddef>

#include <lodepng/lodepng.h>

//Internal interface
bool TextureData::Read(std::istream &stream)
{
	//Get length of stream
	stream.seekg(0, stream.end);
	size_t size = stream.tellg();
	stream.seekg(0, stream.beg);
	
	//Read data
	char *buffer = new char[size];
	if (buffer == nullptr)
		return error.Push("Failed to allocate stream buffer");
	stream.read(buffer, size);
	
	//Process data as PNG
	unsigned int decode_error;
	if ((decode_error = lodepng_decode32((unsigned char**)&data, &width, &height, (const unsigned char*)buffer, size)) != 0)
		return error.Push(lodepng_error_text(decode_error));
	
	//Delete file buffer
	delete[] buffer;
	
	return false;
}

//Constructors and destructor
TextureData::TextureData(std::string path)
{
	//Read from path as a file
	std::ifstream stream(path, std::ifstream::binary);
	if (!stream)
	{
		error.Push("Failed to open texture file at " + path);
		return;
	}
	Read(stream);
}

TextureData::TextureData(std::istream &stream)
{
	//Read stream
	Read(stream);
}

TextureData::~TextureData()
{
	//Delete loaded data
	free(data);
}

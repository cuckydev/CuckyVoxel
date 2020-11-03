#pragma once
#include <string>
#include <istream>
#include <cstdint>

#include <Common/Util/Error.h>

//Texture types
class TextureData
{
	private:
		//Error
		Error error;
		
	public:
		//Texture information
		uint8_t *data = nullptr;
		unsigned int width, height;
		
	private:
		//Internal interface
		bool Read(std::istream &stream);
		
	public:
		//Constructors and destructor
		TextureData(std::string path);
		TextureData(std::istream &stream);
		~TextureData();
		
		//Get error
		const Error &GetError() const { return error; }
};

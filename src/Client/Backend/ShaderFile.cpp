#include "ShaderFile.h"
#include <fstream>
#include <cstdint>
#include <cstddef>

namespace Backend
{
	namespace Render
	{
		//Stream helper function
		static std::string stream_read_until(std::istream &stream, std::string delimiter)
		{
			std::string cr;
			char delim = *(delimiter.rbegin());
			size_t sz = delimiter.size(), tot;
			do
			{
				std::string temp;
				std::getline(stream, temp, delim);
				cr += temp + delim;
				tot = cr.size();
			} while ((tot < sz) || (cr.substr(tot - sz, sz) != delimiter));
			return cr.substr(0, tot - sz);
		}
		
		//Internal interface
		bool ShaderFile::Read(std::istream &stream)
		{
			//Ensure stream is good
			if (!stream.good())
			{
				error.Push("Attempt to read stream that isn't good");
				return true;
			}
			
			//Read source
			for (size_t i = 0; i < ShaderLanguage_Num; i++)
			{
				//Seek to vertex source
				stream.seekg(i * 2, stream.beg);
				uint16_t off = stream.get() | (stream.get() << 8);
				stream.seekg(off, stream.beg);
				
				//Read source
				src.push_back({
					stream_read_until(stream, "\xFF"), //vert_src
					stream_read_until(stream, "\xFF"), //frag_src
				});
			}
			return false;
		}
		
		//Constructor and destructor
		ShaderFile::ShaderFile(std::istream stream)
		{
			//Read stream
			Read(stream);
		}
		
		ShaderFile::ShaderFile(std::string path)
		{
			//Open file then read stream
			std::ifstream stream = std::ifstream(path, std::ifstream::binary);
			if (!stream.is_open())
			{
				error.Push("Failed to open shader file at " + path);
				return;
			}
			Read(stream);
			stream.close();
		}
		
		ShaderFile::~ShaderFile()
		{
			return;
		}
		
		//Shader file interface
		bool ShaderFile::GetSource(ShaderLanguage language, std::string &vert_src, std::string &frag_src) const
		{
			vert_src = src[language].vert_src;
			frag_src = src[language].frag_src;
			return false;
		}
	}
}

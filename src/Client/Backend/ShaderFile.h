#pragma once
#include <istream>
#include <string>
#include <vector>
#include <Common/Util/Error.h>

namespace Backend
{
	namespace Render
	{
		//Shader file types
		enum ShaderLanguage
		{
			ShaderLanguage_GLSL,
			ShaderLanguage_Num,
		};
		
		struct ShaderSrc
		{
			std::string vert_src, frag_src;
		};
		
		//Shader file class
		class ShaderFile
		{
			private:
				//Error
				Error error;
				
				//File information
				std::vector<ShaderSrc> src;
				
			private:
				//Internal interface
				bool Read(std::istream &stream);
				
			public:
				//Constructor and destructor
				ShaderFile(std::istream stream);
				ShaderFile(std::string path);
				~ShaderFile();
				
				//Shader file interface
				bool GetSource(ShaderLanguage language, std::string &vert_src, std::string &frag_src) const;
				
				//Get error
				const Error &GetError() const { return error; }
		};
	}
}

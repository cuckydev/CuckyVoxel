#pragma once
#include <string>
#include <Common/Util/Error.h>

namespace Backend
{
	namespace Render
	{
		//Render types
		struct Config
		{
			std::string title;
			bool fullscreen, resizable;
			unsigned width, height;
		};
		
		enum ShaderLanguage
		{
			ShaderLanguage_GLSL,
			ShaderLanguage_Num,
		};
		
		//Shader base class
		class Shader
		{
			protected:
				//Error
				Error error;
				
			public:
				//Virtual destructor
				virtual ~Shader() {}
				
				//Shader interface
				virtual bool Bind() = 0;
				virtual bool Unbind() = 0;
				
				virtual bool SetUniform(std::string loc, int i0) = 0;
				virtual bool SetUniform(std::string loc, int i0, int i1) = 0;
				virtual bool SetUniform(std::string loc, int i0, int i1, int i2) = 0;
				virtual bool SetUniform(std::string loc, int i0, int i1, int i2, int i3) = 0;
				virtual bool SetUniform(std::string loc, float f0) = 0;
				virtual bool SetUniform(std::string loc, float f0, float f1) = 0;
				virtual bool SetUniform(std::string loc, float f0, float f1, float f2) = 0;
				virtual bool SetUniform(std::string loc, float f0, float f1, float f2, float f3) = 0;
				virtual bool SetUniform(std::string loc, int count, float *ptr) = 0;
				
				//Get error
				const Error &GetError() const { return error; }
		};
		
		//Render base class
		class Render_Base
		{
			protected:
				//Error
				Error error;
				
			public:
				//Virtual destructor
				virtual ~Render_Base() {}
				
				//Render interface
				virtual bool SetConfig(const Config config) = 0;
				virtual ShaderLanguage GetShaderLanguage() = 0;
				
				//Get error
				const Error &GetError() const { return error; }
		};
	}
}

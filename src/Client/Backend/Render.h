#pragma once
#include <string>
#include <vector>
#include <Common/Util/Error.h>

namespace Backend
{
	namespace Render
	{
		//General render types
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
		
		//Vertex types
		const unsigned VERTEX_POS_ELEMENTS = 3;
		const unsigned VERTEX_UV_ELEMENTS = 2;
		const unsigned VERTEX_NORMAL_ELEMENTS = 3;
		const unsigned VERTEX_COLOUR_ELEMENTS = 4;
		const unsigned VERTEX_ELEMENTS = VERTEX_POS_ELEMENTS + VERTEX_UV_ELEMENTS + VERTEX_NORMAL_ELEMENTS + VERTEX_COLOUR_ELEMENTS;
		
		struct Vertex
		{
			float x, y, z; //Position
			float u, v; //Texture Coordinates
			float nx, ny, nz; //Normal
			float r, g, b, a; //Vertex colour
		};
		
		//Mesh base class
		class Mesh
		{
			protected:
				//Error
				Error error;
				
			public:
				//Virtual destructor
				virtual ~Mesh() {}
				
				//Mesh interface
				virtual bool SetData(const std::vector<Vertex> vert, const std::vector<unsigned int> ind) = 0;
				virtual bool Draw() = 0;
				
				//Get error
				const Error &GetError() const { return error; }
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
				
				virtual bool EndFrame() = 0;
				
				virtual Mesh *NewMesh() = 0;
				virtual Mesh *NewMesh(const std::vector<Vertex> vert, const std::vector<unsigned int> ind) = 0;
				virtual Shader *NewShader(std::string vert_src, std::string frag_src) = 0;
				
				//Get error
				const Error &GetError() const { return error; }
		};
	}
}

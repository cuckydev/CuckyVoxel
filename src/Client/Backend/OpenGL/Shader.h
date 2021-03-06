#pragma once
#include <GL/glew.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include "../Render.h"

namespace Backend
{
	namespace Render
	{
		//Shader class
		class Shader_OpenGL : public Shader
		{
			private:
				//GL objects
				GLuint program_id = 0;
				GLuint vertex_id = 0;
				GLuint fragment_id = 0;
				
				//Uniform map
				std::unordered_map<std::string, GLint> uniform_map;
				
			private:
				//Internal interface
				bool CompileSource(std::string_view source, GLuint &id, const GLenum type);
				bool CompileShader(std::string vert_src, std::string frag_src);
				GLint GetUniformLocation(std::string loc);
				
			public:
				//Constructor and destructor
				Shader_OpenGL(std::string vert_src, std::string frag_src);
				Shader_OpenGL(const ShaderFile &file);
				~Shader_OpenGL() override;
				
				//Shader interface
				bool Bind() override;
				bool Unbind() override;
				
				bool SetUniform(std::string loc, int i0) override;
				bool SetUniform(std::string loc, int i0, int i1) override;
				bool SetUniform(std::string loc, int i0, int i1, int i2) override;
				bool SetUniform(std::string loc, int i0, int i1, int i2, int i3) override;
				bool SetUniform(std::string loc, float f0) override;
				bool SetUniform(std::string loc, float f0, float f1) override;
				bool SetUniform(std::string loc, float f0, float f1, float f2) override;
				bool SetUniform(std::string loc, float f0, float f1, float f2, float f3) override;
				bool SetUniform(std::string loc, int count, float *ptr) override;
		};
	}
}

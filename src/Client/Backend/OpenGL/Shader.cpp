#include "Shader.h"

#include <iostream>

namespace Backend
{
	namespace Render
	{
		//Internal interface
		bool Shader_OpenGL::CompileSource(std::string_view source, GLuint &id, const GLenum type)
		{
			//Create our shader instance
			id = glCreateShader(type);
			
			//Compile shader
			const GLchar* const source_ptr = source.data();
			const GLint source_length = source.length();
			glShaderSource(id, 1, &source_ptr, &source_length);
			glCompileShader(id);
			
			//Check for shader compilation errors
			GLint log_length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
			
			if (log_length > 1)
			{
				std::string info_log(log_length, 0);
				glGetShaderInfoLog(id, log_length, nullptr, info_log.data());
				return error.Push(info_log);
			}
			
			return false;
		}
		
		bool Shader_OpenGL::CompileShader(std::string vert_src, std::string frag_src)
		{
			//Compile our vertex and fragment shaders
			if (CompileSource(vert_src, vertex_id, GL_VERTEX_SHADER))
				return true;
			if (CompileSource(frag_src, fragment_id, GL_FRAGMENT_SHADER))
				return true;
			
			//Create our program instance and link the vertex and fragment shaders
			program_id = glCreateProgram();
			glAttachShader(program_id, vertex_id);
			glAttachShader(program_id, fragment_id);
			glLinkProgram(program_id);
			glValidateProgram(program_id);
			glDetachShader(program_id, vertex_id);
			glDetachShader(program_id, fragment_id);
			
			//Check for program link errors
			GLint log_length;
			glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
			
			if (log_length > 1)
			{
				std::string info_log(log_length, 0);
				glGetProgramInfoLog(program_id, log_length, nullptr, info_log.data());
				return error.Push(info_log);
			}
			
			return false;
		}
		
		GLint Shader_OpenGL::GetUniformLocation(std::string loc)
		{
			//Use previously defined location
			auto key_loc = uniform_map.find(loc);
			if (key_loc != uniform_map.end())
				return key_loc->second;
			
			//Find and remember location
			GLint uniform_loc = glGetUniformLocation(program_id, loc.c_str());
			if (uniform_loc == -1)
			{
				error.Push("Failed to get uniform location of " + loc);
				return -1;
			}
			uniform_map[loc] = uniform_loc;
			return uniform_loc;
		}
		
		//Constructor and destructor
		Shader_OpenGL::Shader_OpenGL(std::string vert_src, std::string frag_src)
		{
			CompileShader(vert_src, frag_src);
		}
		
		Shader_OpenGL::Shader_OpenGL(const ShaderFile &file)
		{
			std::string vert_src, frag_src;
			if (file.GetSource(ShaderLanguage_GLSL, vert_src, frag_src))
				return;
			CompileShader(vert_src, frag_src);
		}
		
		Shader_OpenGL::~Shader_OpenGL()
		{
			//Delete shaders and program
			if (program_id != 0)
				glDeleteProgram(program_id);
			if (vertex_id != 0)
				glDeleteShader(vertex_id);
			if (fragment_id != 0)
				glDeleteShader(fragment_id);
		}
		
		//Shader interface
		bool Shader_OpenGL::Bind()
		{
			glUseProgram(program_id);
			return false;
		}
		
		bool Shader_OpenGL::Unbind()
		{
			glUseProgram(0);
			return false;
		}
		
		#define SETUNIFORM_GET_LOC_ID() \
			GLint loc_id = GetUniformLocation(loc); \
			if (loc_id == -1) \
				return true;
		
		bool Shader_OpenGL::SetUniform(std::string loc, int i0)
		{
			SETUNIFORM_GET_LOC_ID()
			glUniform1i(loc_id, i0);
			return false;
		}
		
		bool Shader_OpenGL::SetUniform(std::string loc, int i0, int i1)
		{
			SETUNIFORM_GET_LOC_ID()
			glUniform2i(loc_id, i0, i1);
			return false;
		}
		
		bool Shader_OpenGL::SetUniform(std::string loc, int i0, int i1, int i2)
		{
			SETUNIFORM_GET_LOC_ID()
			glUniform3i(loc_id, i0, i1, i2);
			return false;
		}
		
		bool Shader_OpenGL::SetUniform(std::string loc, int i0, int i1, int i2, int i3)
		{
			SETUNIFORM_GET_LOC_ID()
			glUniform4i(loc_id, i0, i1, i2, i3);
			return false;
		}
		
		bool Shader_OpenGL::SetUniform(std::string loc, float f0)
		{
			SETUNIFORM_GET_LOC_ID()
			glUniform1f(loc_id, f0);
			return false;
		}
		
		bool Shader_OpenGL::SetUniform(std::string loc, float f0, float f1)
		{
			SETUNIFORM_GET_LOC_ID()
			glUniform2f(loc_id, f0, f1);
			return false;
		}
		
		bool Shader_OpenGL::SetUniform(std::string loc, float f0, float f1, float f2)
		{
			SETUNIFORM_GET_LOC_ID()
			glUniform3f(loc_id, f0, f1, f2);
			return false;
		}
		
		bool Shader_OpenGL::SetUniform(std::string loc, float f0, float f1, float f2, float f3)
		{
			SETUNIFORM_GET_LOC_ID()
			glUniform4f(loc_id, f0, f1, f2, f3);
			return false;
		}
		
		bool Shader_OpenGL::SetUniform(std::string loc, int count, float *ptr)
		{
			SETUNIFORM_GET_LOC_ID()
			glUniformMatrix4fv(loc_id, count, GL_FALSE, ptr);
			return false;
		}
	}
}

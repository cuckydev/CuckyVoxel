#include "Shader.h"

namespace Backend
{
	namespace Render
	{
		//Internal interface
		bool Shader_OpenGL::CompileSource(std::string source, GLuint *id, const GLenum type)
		{
			//Create our shader instance
			GLuint new_id = glCreateShader(type);
			*id = new_id;
			
			//Compile shader
			const char *source_cstr = source.c_str();
			glShaderSource(new_id, 1, &source_cstr, nullptr);
			glCompileShader(new_id);
			
			//Test for shader compilation errors
			GLint result;
			glGetShaderiv(new_id, GL_COMPILE_STATUS, &result);
			
			if (result == GL_FALSE)
			{
				//Get shader error length
				GLint length;
				glGetShaderiv(new_id, GL_INFO_LOG_LENGTH, &length);
				
				//Get shader error
				char *message = new char[length];
				glGetShaderInfoLog(new_id, length, nullptr, message);
				
				//Return error
				error.Push(message);
				delete[] message;
				return true;
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
		Shader_OpenGL::Shader_OpenGL(std::string src_vert, std::string src_frag)
		{
			//Compile our vertex and fragment shaders
			if (CompileSource(src_vert, &vertex_id, GL_VERTEX_SHADER))
				return;
			if (CompileSource(src_frag, &fragment_id, GL_FRAGMENT_SHADER))
				return;
			
			//Create our program instance and attach the vertex and fragment shaders, then link
			program_id = glCreateProgram();
			glAttachShader(program_id, vertex_id);
			glAttachShader(program_id, fragment_id);
			glLinkProgram(program_id);
			
			//Test for program linking errors
			GLint result;
			glGetShaderiv(program_id, GL_LINK_STATUS, &result);
			
			if (result == GL_FALSE)
			{
				//Get shader error length
				GLint length;
				glGetShaderiv(program_id, GL_INFO_LOG_LENGTH, &length);
				
				//Get shader error
				char *message = new char[length];
				glGetShaderInfoLog(program_id, length, nullptr, message);
				
				//Return error
				error.Push(message);
				delete[] message;
				return;
			}
			
			//Validate program
			glValidateProgram(program_id);
			
			//Test for program validation errors
			glGetShaderiv(program_id, GL_VALIDATE_STATUS, &result);
			
			if (result == GL_FALSE)
			{
				//Get shader error length
				GLint length;
				glGetShaderiv(program_id, GL_INFO_LOG_LENGTH, &length);
				
				//Get shader error
				char *message = new char[length];
				glGetShaderInfoLog(program_id, length, nullptr, message);
				
				//Return error
				error.Push(message);
				delete[] message;
				return;
			}
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

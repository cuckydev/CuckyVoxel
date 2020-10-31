#include "Texture.h"

namespace Backend
{
	namespace Render
	{
		//Constructors and destructor
		Texture_OpenGL::Texture_OpenGL()
		{
			//Create texture object
			glGenTextures(1, &id);
		}
		
		Texture_OpenGL::Texture_OpenGL(const uint8_t *data, const unsigned int width, const unsigned int height)
		{
			//Create texture object and use given data
			glGenTextures(1, &id);
			SetData(data, width, height);
		}
		
		Texture_OpenGL::~Texture_OpenGL()
		{
			//Delete texture object
			glDeleteTextures(1, &id);
		}
		
		//Set texture data function
		bool Texture_OpenGL::SetData(const uint8_t *data, const unsigned int width, const unsigned int height)
		{
			//Bind texture object
			glBindTexture(GL_TEXTURE_2D, id);
			
			//Set texture parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			
			//Use the given buffer
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			
			return false;
		}
	}
}

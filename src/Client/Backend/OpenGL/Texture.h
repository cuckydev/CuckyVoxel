#pragma once
#include <GL/glew.h>
#include "../Render.h"

namespace Backend
{
	namespace Render
	{
		class Texture_OpenGL : public Texture
		{
			private:
				//GL object
				GLuint id;
				
			public:
				//Constructors and destructor
				Texture_OpenGL();
				Texture_OpenGL(const uint8_t *data, const unsigned int width, const unsigned int height);
				~Texture_OpenGL() override;
				
				//Texture interface
				bool SetData(const uint8_t *data, const unsigned int width, const unsigned int height) override;
				bool Bind() override
				{
					glBindTexture(GL_TEXTURE_2D, id);
					return false;
				}
				bool Unbind() override
				{
					glBindTexture(GL_TEXTURE_2D, 0);
					return false;
				}
		};
	}
}

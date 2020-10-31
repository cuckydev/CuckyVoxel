#include "Render.h"

#include <GL/glew.h>

namespace Backend
{
	namespace Render
	{
		//Internal interface
		bool Render_OpenGL::InitOpenGL()
		{
			//Initialize GLEW
			GLenum glew_error = glewInit();
			if (glew_error != GLEW_OK)
				return error.Push((const char*)glewGetErrorString(glew_error));
			
			//Initialize OpenGL state
			glActiveTexture(GL_TEXTURE0);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			
			return false;
		}
		
		void Render_OpenGL::SetViewport(unsigned int width, unsigned int height)
		{
			//Use given dimensions
			glViewport(0, 0, width, height);
		}
		
		//Render interface
		void Render_OpenGL::SetDepthCompare(DepthCompare depth_compare)
		{
			static GLenum dcomp_map[] = {
				/*DepthCompare_Never*/ GL_NEVER,
				/*DepthCompare_Less*/ GL_LESS,
				/*DepthCompare_Equal*/ GL_EQUAL,
				/*DepthCompare_LessEqual*/ GL_LEQUAL,
				/*DepthCompare_Greater*/ GL_GREATER,
				/*DepthCompare_NotEqual*/ GL_NOTEQUAL,
				/*DepthCompare_GreaterEqual*/ GL_GEQUAL,
				/*DepthCompare_Always*/ GL_ALWAYS,
			};
			
			glDepthFunc(dcomp_map[depth_compare]);
		}
		
		bool Render_OpenGL::ClearColour(float r, float g, float b)
		{
			glClearColor(r, g, b, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			return false;
		}
		
		bool Render_OpenGL::ClearDepth(float depth)
		{
			glClearDepth(depth);
			glClear(GL_DEPTH_BUFFER_BIT);
			return false;
		}
	}
}

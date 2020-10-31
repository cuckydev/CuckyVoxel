#include "Render.h"

#include <GL/glew.h>

namespace Backend
{
	namespace Render
	{
		//Constructors and destructor
		Render_SDL2::Render_SDL2()
		{
			return;
		}
		
		Render_SDL2::Render_SDL2(const Config config)
		{
			//Use given config
			SetConfig(config);
		}
		
		Render_SDL2::~Render_SDL2()
		{
			//Delete SDL window and OpenGL context
			SDL_GL_DeleteContext(gl_context);
			SDL_DestroyWindow(window);
		}
		
		//Render interface
		bool Render_SDL2::SetConfig(const Config config)
		{
			//Delete previous window
			SDL_GL_DeleteContext(gl_context);
			SDL_DestroyWindow(window);
			
			//Use OpenGL 3.3 core and set other GL attributes
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
			
			//Create new window
			if ((window = SDL_CreateWindow(
					config.title.c_str(), //title
					SDL_WINDOWPOS_CENTERED, //x
					SDL_WINDOWPOS_CENTERED, //y
					config.width, //width
					config.height, //height
					SDL_WINDOW_OPENGL | (config.resizable ? SDL_WINDOW_RESIZABLE : 0) //flags
				)) == nullptr)
				return error.Push(SDL_GetError());
			
			//Create new OpenGL context
			if (SDL_GL_CreateContext(window) == nullptr)
				return error.Push(SDL_GetError());
			
			//Use VSync
			if (SDL_GL_SetSwapInterval(-1) < 0)
				SDL_GL_SetSwapInterval(1);
			
			//Initialize OpenGL
			if (InitOpenGL())
				return true;
			
			return false;
		}
		
		bool Render_SDL2::EndFrame()
		{
			SDL_GL_SwapWindow(window);
			return false;
		}
	}
}

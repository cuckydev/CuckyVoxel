#include "Render.h"

#include "SDL_mouse.h"

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
			
			//Use OpenGL 3.2 core and set other GL attributes
			if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) < 0 ||
			    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG) < 0 ||
			    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3) < 0 ||
			    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2) < 0)
			    return error.Push(SDL_GetError());
			
			//Create new window
			if ((window = SDL_CreateWindow(
					config.title.c_str(), //title
					SDL_WINDOWPOS_CENTERED, //x
					SDL_WINDOWPOS_CENTERED, //y
					config.width, //width
					config.height, //height
					SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | (config.resizable ? SDL_WINDOW_RESIZABLE : 0) //flags
				)) == nullptr)
				return error.Push(SDL_GetError());
			
			//Create new OpenGL context
			if ((gl_context = SDL_GL_CreateContext(window)) == nullptr ||
			    SDL_GL_MakeCurrent(window, gl_context) < 0)
				return error.Push(SDL_GetError());
			
			
			//Use VSync
			if (SDL_GL_SetSwapInterval(-1) < 0)
				SDL_GL_SetSwapInterval(1);
			
			//Initialize OpenGL
			if (InitOpenGL())
				return true;
			
			return false;
		}
		
		unsigned int Render_SDL2::GetWidth()
		{
			int width;
			SDL_GL_GetDrawableSize(window, &width, nullptr);
			return width;
		}
		
		unsigned int Render_SDL2::GetHeight()
		{
			int height;
			SDL_GL_GetDrawableSize(window, nullptr, &height);
			return height;
		}
		
		void Render_SDL2::LockMouse()
		{
			SDL_SetRelativeMouseMode(SDL_TRUE);
			SDL_WarpMouseInWindow(window, GetWidth() / 2, GetHeight() / 2);
		}
		
		void Render_SDL2::UnlockMouse()
		{
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}
		
		bool Render_SDL2::EndFrame()
		{
			//Swap window
			SDL_GL_SwapWindow(window);
			
			//Use window dimensions
			SetViewport(GetWidth(), GetHeight());
			
			return false;
		}
	}
}

#include "Render.h"

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
			SDL_DestroyWindow(window);
		}
		
		//Render interface
		bool Render_SDL2::SetConfig(const Config config)
		{
			//Delete previous window
			SDL_DestroyWindow(window);
			
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
			
			return false;
		}
	}
}

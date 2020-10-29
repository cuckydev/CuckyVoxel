#include <SDL.h>
#include <iostream>

#include "Core.h"

namespace Backend
{
	namespace Core
	{
		//Constructors and destructor
		Core_SDL2::Core_SDL2()
		{
			return;
		}
		
		Core_SDL2::Core_SDL2(const Config config)
		{
			//Use given config
			SetConfig(config);
		}
		
		Core_SDL2::~Core_SDL2()
		{
			//Quit SDL2
			SDL_Quit();
		}
		
		//Core interface
		bool Core_SDL2::SetConfig(const Config config)
		{
			//Initialize SDL2
			SDL_Init(SDL_INIT_AUDIO |
			         SDL_INIT_VIDEO |
			         SDL_INIT_JOYSTICK |
			         SDL_INIT_HAPTIC |
			         SDL_INIT_GAMECONTROLLER |
			         SDL_INIT_EVENTS);
			return false;
		}
	}
}

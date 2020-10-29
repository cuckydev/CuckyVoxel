#pragma once
#include <SDL_render.h>
#include "../Render.h"

namespace Backend
{
	namespace Render
	{
		class Render_SDL2 : public Render_Base
		{
			private:
				//SDL window and OpenGL context
				SDL_Window *window = nullptr;
				
			public:
				//Constructors and destructor
				Render_SDL2();
				Render_SDL2(const Config config);
				~Render_SDL2() override;
				
				//Render interface
				bool SetConfig(const Config config) override;
		};
	}
}

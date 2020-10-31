#pragma once
#include <SDL_render.h>
#include "../OpenGL/Render.h"

namespace Backend
{
	namespace Render
	{
		class Render_SDL2 : public Render_OpenGL
		{
			private:
				//SDL window and OpenGL context
				SDL_Window *window = nullptr;
				SDL_GLContext gl_context = nullptr; //SDL_GLContext is a pointer type
				
			public:
				//Constructors and destructor
				Render_SDL2();
				Render_SDL2(const Config config);
				~Render_SDL2() override;
				
				//Render interface
				bool SetConfig(const Config config) override;
				
				unsigned int GetWidth() override;
				unsigned int GetHeight() override;
				
				bool EndFrame() override;
		};
	}
}

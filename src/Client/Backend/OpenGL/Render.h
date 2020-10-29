#pragma once
#include "../Render.h"

namespace Backend
{
	namespace Render
	{
		class Render_OpenGL : public Render_Base
		{
			public:
				//Virtual destructor
				virtual ~Render_OpenGL() {}
				
				//Render interface
				ShaderLanguage GetShaderLanguage() override { return ShaderLanguage_GLSL; }
		};
	}
}

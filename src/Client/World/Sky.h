#pragma once
#include "../Backend/Render.h"

#include <Common/Util/Error.h>
#include <Common/Util/ColourSpace.h>

#include <glm/mat4x4.hpp>

namespace World
{
	//Sky types
	struct SkyState
	{
		float temperature;
		float celestial_angle;
		float blend_factor;
	};
	
	//Sky class
	class Sky
	{
		private:
			//Error
			Error error;
			
			//Sky state
			SkyState sky_state;
			
			//Renderer and render objects
			Backend::Render::Render *render;
			
			Backend::Render::Shader *sky_shader = nullptr;
			Backend::Render::Mesh *ceiling_mesh = nullptr;
			//Backend::Render::Mesh *void_mesh = nullptr;
			
			int cloud_width;
			int cloud_height;
			uint8_t *cloud_map = nullptr;
			
			float cloud_x;
			Backend::Render::Mesh *cloud_mesh[16] = {};
			
		public:
			//Constructor and destructor
			Sky(Backend::Render::Render *_render);
			~Sky();
			
			//Sky interface
			void SetSkyState(const SkyState &_sky_state);
			
			ColourSpace::RGB GetSkyColour();
			ColourSpace::RGB GetFogColour();
			ColourSpace::RGB GetAtmosphereColour();
			ColourSpace::RGB GetCloudColour();
			
			bool Render(glm::mat4 projection, glm::mat4 view, glm::dvec3 cam_pos);
			
			//Get error
			const Error &GetError() const { return error; }
	};
}

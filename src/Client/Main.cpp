#include "Main.h"
#include <iostream>

#include "Backend/Backend.h"
#include "Backend/ShaderFile.h"
#include "Texture.h"

#include "World/World.h"

#include <Common/World/WorldPosition.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

std::string executable_dir;

int main(int argc, char *argv[])
{
	//Get executable directory
	if (argc > 0)
	{
		executable_dir = argv[0];
		executable_dir = executable_dir.substr(0, executable_dir.find_last_of("/\\") + 1);
	}
	else
	{
		executable_dir = "./";
	}
	
	//Initialize backend
	static const Backend::Config config = {
		{}, //core_config
		{ //render_config
			"CuckyVoxel", //title
			false, //fullscreen
			true, //resizable
			854, //width
			480, //height
		},
		{}, //event_config
	};;
	
	Backend::Backend *backend = new Backend::Backend(Backend::Type_SDL2, config);
	if (backend == nullptr)
	{
		std::cout << "Failed to create backend instance" << std::endl;
		return 1;
	}
	else if (backend->GetError())
	{
		std::cout << "backend error " << backend->GetError() << std::endl;
		return 1;
	}
	
	//Get subsystems
	Backend::Render::Render *render = backend->GetRender();
	Backend::Event::Event *event = backend->GetEvent();
	
	//Initialize render state
	render->SetDepthCompare(Backend::Render::DepthCompare_LessEqual);
	render->LockMouse();
	
	//Create world
	World::World world(render);
	
	//Render scene
	glm::dvec3 cam_pos = {8.0f, 130.0f, 8.0f};
	double cam_turn = 0.0f;
	double cam_look = -1.5f;
	glm::dvec3 cam_up = {0.0f, 1.0f,  0.0f};
	bool w = false, a = false, s = false, d = false, e = false;
	
	while (1)
	{
		//Handle events
		Backend::Event::EventData event_data;
		bool quit = false;
		while (event->PollEvent(event_data))
		{
			switch (event_data.type)
			{
				case Backend::Event::EventType_Quit:
					quit = true;
					break;
				case Backend::Event::EventType_InputBool:
					switch (event_data.input_bool.device)
					{
						case Backend::Event::InputDevice_Keyboard:
							switch (event_data.input_bool.code)
							{
								case Backend::Event::InputCode_W:
									w = event_data.input_bool.value;
									break;
								case Backend::Event::InputCode_A:
									a = event_data.input_bool.value;
									break;
								case Backend::Event::InputCode_S:
									s = event_data.input_bool.value;
									break;
								case Backend::Event::InputCode_D:
									d = event_data.input_bool.value;
									break;
								case Backend::Event::InputCode_E:
									e = event_data.input_bool.value;
									break;
							}
							break;
					}
					break;
				case Backend::Event::EventType_InputFloat:
					switch (event_data.input_float.device)
					{
						case Backend::Event::InputDevice_Mouse:
							switch (event_data.input_float.code)
							{
								case Backend::Event::InputCode_Mouse_X:
									cam_turn += event_data.input_float.rel_value * 0.01f;
									break;
								case Backend::Event::InputCode_Mouse_Y:
									cam_look += event_data.input_float.rel_value * -0.01f;
									if (cam_look > 1.5f)
										cam_look = 1.5f;
									if (cam_look < -1.5f)
										cam_look = -1.5f;
									break;
							}
							break;
					}
					break;
			}
		}
		if (quit)
			break;
			
		//Move camera
		glm::dvec3 cam_dir = {
			-glm::sin(cam_turn) * glm::cos(cam_look),
			glm::sin(cam_look),
			glm::cos(cam_turn) * glm::cos(cam_look),
		};
		
		double cam_speed = 0.25f;
		if (w)
			cam_pos += cam_dir * cam_speed;
		if (s)
			cam_pos -= cam_dir * cam_speed;
		if (a)
			cam_pos -= glm::normalize(glm::cross(cam_dir, cam_up)) * cam_speed;
		if (d)
			cam_pos += glm::normalize(glm::cross(cam_dir, cam_up)) * cam_speed;
		
		//Tick world
		const World::ChunkPosition &cam_chunk = World::WorldToChunkPosition(cam_pos);
		if (!e)
		{
			#define GEN_RAD 3
			for (int x = -GEN_RAD; x <= GEN_RAD; x++)
				for (int z = -GEN_RAD; z <= GEN_RAD; z++)
					world.GenerateChunk({cam_chunk.x + x, cam_chunk.z + z});
		}
		if (world.Update())
		{
			std::cout << world.GetError() << std::endl;
			return 1;
		}
		
		//Render sky
		render->ClearColour(0.7529412f, 0.8470588f, 1.0f);
		render->ClearDepth(1.0f);
		
		//Render scene
		if (world.Render(cam_pos, cam_dir))
		{
			std::cout << world.GetError() << std::endl;
			return 1;
		}
		
		//End frame
		if (render->EndFrame())
			break;
	}
	
	//Delete backend
	delete backend;
	
	return 0;
}

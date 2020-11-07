#include "Main.h"
#include <iostream>

#include "Backend/Backend.h"

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
	if (world.GetError())
	{
		std::cout << world.GetError() << std::endl;
		return 1;
	}
	std::cout << world.GetSeed() << std::endl;
	
	//Render scene (all this code is temporary)
	glm::dvec3 cam_pos = {0.0, 70.0, 0.0};
	double cam_turn = glm::radians(-119.23);
	double cam_look = glm::radians(8.297);
	double cam_speed = 1.0;
	glm::dvec3 cam_up = {0.0, 1.0,  0.0};
	bool w = false, a = false, s = false, d = false;
	
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
									if (event_data.input_bool.value)
										render->UnlockMouse();
									else
										render->LockMouse();
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
		
		if (w)
			cam_pos += cam_dir * cam_speed;
		if (s)
			cam_pos -= cam_dir * cam_speed;
		if (a)
			cam_pos -= glm::normalize(glm::cross(cam_dir, cam_up)) * cam_speed;
		if (d)
			cam_pos += glm::normalize(glm::cross(cam_dir, cam_up)) * cam_speed;
		
		//Tick world
		if (world.Update(cam_pos))
		{
			std::cout << world.GetError() << std::endl;
			return 1;
		}
		
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

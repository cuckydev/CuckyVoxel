#include <iostream>
#include "Backend/Backend.h"

#include "Backend/ShaderFile.h"
#include "Texture.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <Common/World/World.h>

#include <GL/glew.h>

int main(int argc, char *argv[])
{
	//Get executable directory
	std::string executable_dir;
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
	
	//Shader
	Backend::Render::ShaderFile shader_file(executable_dir + "Data/Shader/Generic.shd");
	if (shader_file.GetError())
	{
		std::cout << "shader file error " << shader_file.GetError() << std::endl;
		return 1;
	}
	
	Backend::Render::Shader *shader = render->NewShader(shader_file);
	if (shader == nullptr)
	{
		std::cout << "Failed to create shader instance" << std::endl;
		return 1;
	}
	else if (shader->GetError())
	{
		std::cout << "shader error " << shader->GetError() << std::endl;
		return 1;
	}
	
	//Initialize render state
	render->SetDepthCompare(Backend::Render::DepthCompare_LessEqual);
	render->LockMouse();
	
	//Game state
	World::World world;
	
	World::Chunk *chunk[8][8];
	Backend::Render::Mesh *chunk_mesh[8][8];
	for (int z = 0; z < 8; z++)
	{
		for (int x = 0; x < 8; x++)
		{
			//Generate chunk
			chunk[z][x] = &((world.GetChunkManager())->GenerateChunk({x + 2, z + 4}));
		}
	}
	
	for (int z = 0; z < 8; z++)
	{
		for (int x = 0; x < 8; x++)
		{
			//Create chunk mesh
			World::ChunkMeshData mesh_data = chunk[z][x]->GetMeshData();
			
			std::vector<Backend::Render::Vertex> vert;
			for (auto &i : mesh_data.vert)
			{
				vert.push_back({
					{i.x, i.y, i.z},
					{0.0f, 0.0f},
					{0.0f, 0.0f, 0.0f},
					{i.r, i.g, i.b, 1.0f},
				});
			}
			
			Backend::Render::Mesh *mesh = render->NewMesh(vert, mesh_data.ind);
			if (mesh == nullptr)
			{
				std::cout << "Failed to create mesh instance" << std::endl;
				return 1;
			}
			else if (mesh->GetError())
			{
				std::cout << "mesh error " << mesh->GetError() << std::endl;
				return 1;
			}
			
			chunk_mesh[z][x] = mesh;
		}
	}
	
	//Render scene
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 model;
	
	glm::vec3 cam_pos = {8.0f, 130.0f, 8.0f};
	float cam_turn = 0.0f;
	float cam_look = -1.5f;
	glm::vec3 cam_up = {0.0f, 1.0f,  0.0f};
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
			
		//Get new projection matrix
		projection = glm::perspective(glm::radians(70.0f), (float)render->GetWidth() / (float)render->GetHeight(), 0.1f, 1024.f);
		
		//Move camera
		glm::vec3 cam_dir = {
			-glm::sin(cam_turn) * glm::cos(cam_look),
			glm::sin(cam_look),
			glm::cos(cam_turn) * glm::cos(cam_look),
		};
		
		float cam_speed = 0.25f;
		if (w)
			cam_pos += cam_dir * cam_speed;
		if (s)
			cam_pos -= cam_dir * cam_speed;
		if (a)
			cam_pos -= glm::normalize(glm::cross(cam_dir, cam_up)) * cam_speed;
		if (d)
			cam_pos += glm::normalize(glm::cross(cam_dir, cam_up)) * cam_speed;
		
		//Use generic shader
		shader->Bind();
		shader->SetUniform("u_projection", 1, &(projection[0][0]));
		
		//Render sky
		view = glm::lookAt({}, cam_dir, {0.0f, 1.0f, 0.0f});
		shader->SetUniform("u_view", 1, &(view[0][0]));
		
		render->ClearColour(0.7529412f, 0.8470588f, 1.0f);
		render->ClearDepth(1.0f);
		
		//Render scene
		view = glm::lookAt(cam_pos, cam_pos + cam_dir, {0.0f, 1.0f, 0.0f});
		shader->SetUniform("u_view", 1, &(view[0][0]));
		
		for (int z = 0; z < 8; z++)
		{
			for (int x = 0; x < 8; x++)
			{
				model = glm::translate(glm::mat4(1.0f), {CHUNK_DIM * x * 1, 0, CHUNK_DIM * z * 1});
				shader->SetUniform("u_model", 1, &(model[0][0]));
				chunk_mesh[z][x]->Draw();
			}
		}
		
		//End frame
		if (render->EndFrame())
			break;
	}
	
	//Delete render objects
	for (int z = 0; z < 8; z++)
		for (int x = 0; x < 8; x++)
			delete chunk_mesh[z][x];
	delete shader;
	
	//Delete backend
	delete backend;
	
	return 0;
}

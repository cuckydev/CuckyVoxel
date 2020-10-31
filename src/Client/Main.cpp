#include <iostream>
#include "Backend/Backend.h"

#include "Backend/ShaderFile.h"
#include "Texture.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char *argv[])
{
	//Get executable directory
	std::string executable_dir = argv[0];
	executable_dir = executable_dir.substr(0, executable_dir.find_last_of("/\\") + 1);
	
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
	};;
	
	Backend::Backend *backend = new Backend::Backend(Backend::Type_SDL2, config);
	if (backend == nullptr)
	{
		std::cout << "Failed to create backend instance" << std::endl;
		return 1;
	}
	else if (backend->GetError())
	{
		std::cout << backend->GetError() << std::endl;
		return 1;
	}
	
	//Get subsystems
	Backend::Render::Render *render = backend->GetRender();
	Backend::Event::Event *event = backend->GetEvent();
	
	//Mesh
	static const std::vector<Backend::Render::Vertex> vertices = {
		{
			{-0.5f, 0.5f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 1.0f, 1.0f},
		},
		{
			{0.5f, 0.5f, 0.0f},
			{1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{0.0f, 1.0f, 0.0f, 1.0f},
		},
		{
			{0.5f, -0.5f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 0.0f, 1.0f},
			{1.0f, 0.0f, 0.0f, 1.0f},
		},
		{
			{-0.5f, -0.5f, 0.0f},
			{0.0f, 1.0f},
			{0.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 1.0f, 1.0f},
		},
	};
	
	static const std::vector<unsigned int> indices = {
		3, 1, 0,
		1, 3, 2,
	};
	
	Backend::Render::Mesh *mesh = render->NewMesh(vertices, indices);
	if (mesh == nullptr)
	{
		std::cout << "Failed to create mesh instance" << std::endl;
		return 1;
	}
	else if (mesh->GetError())
	{
		std::cout << mesh->GetError() << std::endl;
		return 1;
	}
	
	//Shader
	Backend::Render::ShaderFile shader_file(executable_dir + "Data/Shader/GenericTexture.shd");
	if (shader_file.GetError())
	{
		std::cout << shader_file.GetError() << std::endl;
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
		std::cout << shader->GetError() << std::endl;
		return 1;
	}
	
	//Texture
	Texture::Data tex_data;
	if (Texture::ReadTexture(tex_data, executable_dir + "Data/Texture/Test.png"))
	{
		std::cout << tex_data.error << std::endl;
		return 1;
	}
	
	Backend::Render::Texture *texture = render->NewTexture(tex_data.data, tex_data.width, tex_data.height);
	if (texture == nullptr)
	{
		std::cout << "Failed to create texture instance" << std::endl;
		return 1;
	}
	else if (texture->GetError())
	{
		std::cout << texture->GetError() << std::endl;
		return 1;
	}
	
	//Initialize render state
	render->SetDepthCompare(Backend::Render::DepthCompare_LessEqual);
	
	glm::mat4 projection; 
	glm::mat4 view;
	glm::mat4 model;
	float translate = 0.0f;
	float ang = 0.0f;
	float rad_mag = 0.0f;
	
	float mouse_x = 0.0f;
	float mouse_y = 0.0f;
	
	//Render
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
				case Backend::Event::EventType_InputFloat:
					if (event_data.input_float.device == Backend::Event::InputDevice_Mouse)
					{
						if (event_data.input_float.code == Backend::Event::InputCode_Mouse_X)
							mouse_x = ((float)event_data.input_float.value / render->GetWidth() - 0.5f) * 2.0f;
						if (event_data.input_float.code == Backend::Event::InputCode_Mouse_Y)
							mouse_y = ((float)event_data.input_float.value / render->GetHeight() - 0.5f) * 2.0f;
					}
					break;
			}
		}
		if (quit)
			break;
		
		//Get new projection vector
		projection = glm::perspective(glm::radians(45.0f), (float)render->GetWidth() / (float)render->GetHeight(), 0.1f, 100.f);
		
		//Move camera
		translate += (7.0f - translate) * 0.05f;
		view = glm::lookAt(glm::vec3(translate * -mouse_x, translate * mouse_y, translate), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
		//Render scene
		render->ClearColour(0.5f, 0.5f, 0.5f);
		render->ClearDepth(1.0f);
		
		texture->Bind();
		
		shader->Bind();
		shader->SetUniform("u_projection", 1, &(projection[0][0]));
		shader->SetUniform("u_view", 1, &(view[0][0]));
		
		float rad = (1.0f + glm::sin(rad_mag)) * 1.5f;
		for (int i = 0; i < 8; i++)
		{
			float my_ang = glm::radians(ang + (i * 360.0f / 8));
			model = glm::translate(glm::mat4(1.0f), glm::vec3(glm::cos(my_ang) * rad, 0.0f, glm::sin(my_ang) * rad));
			shader->SetUniform("u_model", 1, &(model[0][0]));
			mesh->Draw();
			model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, glm::cos(my_ang) * rad * 1.25f, glm::sin(my_ang) * rad * 1.25f));
			shader->SetUniform("u_model", 1, &(model[0][0]));
			mesh->Draw();
		}
		
		//End frame
		if (render->EndFrame())
			break;
	}
	
	//Delete backend
	delete backend;
	
	return 0;
}

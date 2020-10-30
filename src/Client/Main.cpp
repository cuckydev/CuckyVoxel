#include <iostream>
#include "Backend/Backend.h"

#include "Backend/ShaderFile.h"

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
	
	Backend::Render::Render_Base *render = backend->GetRender();
	
	//Mesh
	static const std::vector<Backend::Render::Vertex> vertices = {
		{
			{-0.5f, -0.5f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 1.0f, 1.0f},
		},
		{
			{0.5f, -0.5f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{0.0f, 1.0f, 0.0f, 1.0f},
		},
		{
			{0.5f, 0.5f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{1.0f, 0.0f, 0.0f, 1.0f},
		},
		{
			{-0.5f, 0.5f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 1.0f, 1.0f},
		},
	};
	
	static const std::vector<unsigned int> indices = {
		0, 3, 1,
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
	Backend::Render::ShaderFile shader_file(executable_dir + "Data/Shader/Direct.shd");
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
	
	//Render
	while (1)
	{
		shader->Bind();
		mesh->Draw();
		if (render->EndFrame())
			break;
	}
	
	//Delete backend
	delete backend;
	return 0;
}

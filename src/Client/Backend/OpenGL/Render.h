#pragma once
#include "../Render.h"

#include "Mesh.h"
#include "Shader.h"

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
				Mesh *NewMesh() override
				{
					return new Mesh_OpenGL();
				}
				
				Mesh *NewMesh(const std::vector<Vertex> vert, const std::vector<unsigned int> ind) override
				{
					return new Mesh_OpenGL(vert, ind);
				}
				
				Shader *NewShader(const ShaderFile &file) override
				{
					return new Shader_OpenGL(file);
				}
		};
	}
}

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
				ShaderLanguage GetShaderLanguage() override { return ShaderLanguage_GLSL; }
				
				Mesh *NewMesh() override
				{
					return new Mesh_OpenGL();
				}
				
				Mesh *NewMesh(const std::vector<Vertex> vert, const std::vector<unsigned int> ind) override
				{
					return new Mesh_OpenGL(vert, ind);
				}
				
				Shader *NewShader(std::string vert_src, std::string frag_src) override
				{
					return new Shader_OpenGL(vert_src, frag_src);
				}
		};
	}
}

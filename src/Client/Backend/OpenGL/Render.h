#pragma once
#include "../Render.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

namespace Backend
{
	namespace Render
	{
		class Render_OpenGL : public Render_Base
		{
			protected:
				//Internal interface
				bool InitOpenGL();
				void SetViewport(unsigned int width, unsigned int height);
				
			public:
				//Virtual destructor
				virtual ~Render_OpenGL() {}
				
				//Render interface
				void SetDepthCompare(DepthCompare depth_compare) override;
				bool ClearColour(float r, float g, float b) override;
				bool ClearDepth(float depth) override;
				
				Texture *NewTexture() override
				{
					return new Texture_OpenGL();
				}
				
				Texture *NewTexture(const uint8_t *data, const unsigned int width, const unsigned int height) override
				{
					return new Texture_OpenGL(data, width, height);
				}
				
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

#pragma once
#include <GL/glew.h>
#include "../Render.h"

namespace Backend
{
	namespace Render
	{
		class Mesh_OpenGL : public Mesh
		{
			private:
				//GL objects
				GLuint vao_id = 0, vbo_id = 0, eab_id = 0;
				
				//Buffer sizes
				GLsizei vbo_size = 0,     eab_size = 0;
				GLsizei vbo_size_max = 0, eab_size_max = 0;
				
			public:
				//Constructors and destructor
				Mesh_OpenGL();
				Mesh_OpenGL(const std::vector<Vertex> vert, const std::vector<unsigned int> ind);
				~Mesh_OpenGL() override;
				
				//Mesh interface
				bool SetData(const std::vector<Vertex> vert, const std::vector<unsigned int> ind) override;
				bool Draw() override;
		};
	}
}

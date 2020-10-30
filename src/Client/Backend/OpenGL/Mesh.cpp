#include "Mesh.h"

namespace Backend
{
	namespace Render
	{
		//Constructors and destructor
		Mesh_OpenGL::Mesh_OpenGL()
		{
			//Create GL objects
			glGenVertexArrays(1, &vao_id);
			glGenBuffers(1, &vbo_id);
			glGenBuffers(1, &eab_id);
		}
		
		Mesh_OpenGL::Mesh_OpenGL(const std::vector<Vertex> vert, const std::vector<unsigned int> ind)
		{
			//Create GL objects
			glGenVertexArrays(1, &vao_id);
			glGenBuffers(1, &vbo_id);
			glGenBuffers(1, &eab_id);
			
			//Use given data
			SetData(vert, ind);
		}
		
		Mesh_OpenGL::~Mesh_OpenGL()
		{
			//Delete GL objects
			glDeleteVertexArrays(1, &vao_id);
			glDeleteBuffers(1, &vbo_id);
			glDeleteBuffers(1, &eab_id);
		}
		
		//Mesh interface
		bool Mesh_OpenGL::SetData(const std::vector<Vertex> vert, const std::vector<unsigned int> ind)
		{
			//Allocate buffer that holds all data
			auto vert_size = vert.size();
			GLsizei size = vert_size * VERTEX_ELEMENTS;
			GLfloat *data = new GLfloat[size];
			
			//Get positions in buffer that mark each of the attributes
			GLfloat *position = data;
			GLfloat *uv = position + (vert_size * VERTEX_POS_ELEMENTS);
			GLfloat *normal = uv + (vert_size * VERTEX_UV_ELEMENTS);
			GLfloat *colour = normal + (vert_size * VERTEX_NORMAL_ELEMENTS);
			
			//Push data
			GLfloat *datap = data;
			for (auto &i : vert)
			{
				*datap++ = i.x;
				*datap++ = i.y;
				*datap++ = i.z;
			}
			for (auto &i : vert)
			{
				*datap++ = i.u;
				*datap++ = i.v;
			}
			for (auto &i : vert)
			{
				*datap++ = i.nx;
				*datap++ = i.ny;
				*datap++ = i.nz;
			}
			for (auto &i : vert)
			{
				*datap++ = i.r;
				*datap++ = i.g;
				*datap++ = i.b;
				*datap++ = i.a;
			}
			
			//Bind to VAO
			glBindVertexArray(vao_id);
			
			//Send data to VBO
			glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
			
			vbo_size = size;
			if (vbo_size > vbo_size_max)
			{
				glBufferData(GL_ARRAY_BUFFER, vbo_size * sizeof(GLfloat), data, GL_STATIC_DRAW);
				vbo_size_max = vbo_size;
			}
			else
			{
				glBufferSubData(GL_ARRAY_BUFFER, 0, vbo_size * sizeof(GLfloat), data);
			}
			
			//Send indices to EAB
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab_id);
			
			eab_size = ind.size();
			if (eab_size > eab_size_max)
			{
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, eab_size * sizeof(int), ind.data(), GL_STATIC_DRAW);
				eab_size_max = eab_size;
			}
			else
			{
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, eab_size * sizeof(int), ind.data());
			}
			
			//Setup our VAO attributes
			//0 - Position
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)((position - data) * sizeof(GLfloat)));
			
			//1 - Texture coordinate
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)((uv - data) * sizeof(GLfloat)));
			
			//2 - Normal
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)((normal - data) * sizeof(GLfloat)));
			
			//2 - Vertex colour
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)((colour - data) * sizeof(GLfloat)));
			
			delete[] data;
			return false;
		}
		
		bool Mesh_OpenGL::Draw()
		{
			//Bind VAO, VBO, EBO, and draw
			glBindVertexArray(vao_id);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab_id);
			glDrawElements(GL_TRIANGLES, eab_size, GL_UNSIGNED_INT, (void*)0);
			return false;
		}
	}
}

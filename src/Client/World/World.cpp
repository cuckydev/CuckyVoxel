#include "World.h"
#include "../Main.h"
#include <Common/Util/Random.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace World
{
	//Internal interface
	bool World::InitWorld()
	{
		//Shader for world rendering
		Backend::Render::ShaderFile generic_shader_file(executable_dir + "Data/Shader/Generic.shd");
		if (generic_shader_file.GetError())
			return error.Push(generic_shader_file.GetError());
		
		generic_shader = render->NewShader(generic_shader_file);
		if (generic_shader == nullptr)
			return error.Push("Failed to create generic shader instance");
		else if (generic_shader->GetError())
			return error.Push(generic_shader->GetError());
		
		//Create chunk manager
		delete chunk_manager;
		chunk_manager = new ChunkManager(seed);
		if (chunk_manager == nullptr)
			return error.Push("Failed to create chunk manager instance");
		else if (chunk_manager->GetError())
			return error.Push(chunk_manager->GetError());
		
		
		return false;
	}
	
	//Constructors and destructor
	World::World(Backend::Render::Render *_render) : render(_render)
	{
		//Seed based off time and generate
		seed = RandomTimeSeed();
		InitWorld();
	}
	
	World::World(Backend::Render::Render *_render, int64_t _seed) : render(_render)
	{
		//Use given seed and generate
		seed = _seed;
		InitWorld();
	}
	
	World::~World()
	{
		//Delete chunk manager
		delete chunk_manager;
	}
	
	//World interface
	bool World::Update()
	{
		return false;
	}
	
	bool World::Render(glm::vec3 cam_pos, glm::vec3 cam_dir)
	{
		//Update chunk meshes
		for(auto it = chunk_mesh_updates.begin(); it != chunk_mesh_updates.end(); it++)
		{
			//Update chunk mesh
			const ChunkPosition &chunk_pos = it->first;
			
			if (it->second)
			{
				//Get chunk to get mesh from
				Chunk *chunk = chunk_manager->GetChunk(chunk_pos);
				
				if (chunk != nullptr)
				{
					//Get chunk mesh
					auto pos = chunk_meshes.find(chunk_pos);
					
					ChunkMesh *mesh;
					if (pos == chunk_meshes.cend())
						mesh = &chunk_meshes.emplace(std::piecewise_construct,
						                             std::forward_as_tuple(chunk_pos),
						                             std::forward_as_tuple(render)).first->second;
					else
						mesh = &pos->second;
					
					//Generate chunk mesh
					if (mesh->GenerateMesh(chunk->GetMeshData()))
						return error.Push(mesh->GetError());
				}
			}
			else
			{
				//Delete chunk mesh at position
				chunk_meshes.erase(chunk_pos);
			}
		}
		
		chunk_mesh_updates.clear();
		
		//Get projection and view matrices
		glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)render->GetWidth() / (float)render->GetHeight(), 0.1f, 1024.f);
		glm::mat4 view = glm::lookAt(cam_pos, cam_pos + cam_dir, {0.0f, 1.0f, 0.0f});
		
		//Use generic shader
		generic_shader->Bind();
		generic_shader->SetUniform("u_projection", 1, &(projection[0][0]));
		generic_shader->SetUniform("u_view", 1, &(view[0][0]));
		
		//Render chunk meshes
		for(auto it = chunk_meshes.begin(); it != chunk_meshes.end(); it++)
		{
			//Get model position
			const ChunkPosition &chunk_pos = it->first;
			glm::mat4 model = glm::translate(glm::mat4(1.0f), {chunk_pos.x * CHUNK_DIM, 0, chunk_pos.z * CHUNK_DIM});
			generic_shader->SetUniform("u_model", 1, &(model[0][0]));
			
			//Render mesh
			Backend::Render::Mesh *mesh = (it->second).GetMesh();
			if (mesh != nullptr)
				mesh->Draw();
		}
		
		return false;
	}
	
	//World chunk interface
	void World::GenerateChunk(const ChunkPosition &chunk_pos)
	{
		if (!chunk_manager->HasChunk(chunk_pos))
		{
			//Generate chunk at given position and queue update
			chunk_manager->GenerateChunk(chunk_pos);
			chunk_mesh_updates[{chunk_pos.x + 0, chunk_pos.z + 0}] = true;
			chunk_mesh_updates[{chunk_pos.x + 1, chunk_pos.z + 0}] = true; //X+
			chunk_mesh_updates[{chunk_pos.x + 0, chunk_pos.z + 1}] = true; //Z+
			chunk_mesh_updates[{chunk_pos.x - 1, chunk_pos.z + 0}] = true; //X-
			chunk_mesh_updates[{chunk_pos.x + 0, chunk_pos.z - 1}] = true; //Z-
		}
	}
}

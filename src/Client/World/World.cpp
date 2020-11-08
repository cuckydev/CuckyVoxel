#include "World.h"
#include "../Main.h"
#include <Common/Util/Random.h>

#include "../TextureData.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace World
{
	//World constants
	static const int chunk_range = 12;
	
	//Internal interface
	bool World::InitWorld()
	{
		//Load terrain texture
		TextureData terrain_texture_data(executable_dir + "Data/World/Terrain.png");
		if (terrain_texture_data.GetError())
			return error.Push(terrain_texture_data.GetError());
		
		terrain_texture = render->NewTexture(terrain_texture_data.data, terrain_texture_data.width, terrain_texture_data.height);
		if (terrain_texture == nullptr)
			return error.Push("Failed to create terrain texture instance");
		else if (terrain_texture->GetError())
			return error.Push(terrain_texture->GetError());
		
		//Load world shader
		Backend::Render::ShaderFile generic_shader_file(executable_dir + "Data/Shader/GenericWorld.shd");
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
		
		//Create sky
		sky = new Sky(render);
		if (sky == nullptr)
			return error.Push("Failed to create sky instance");
		else if (sky->GetError())
			return error.Push(sky->GetError());
		
		//Start threads
		threads_running = true;
		
		//Start generation thread
		genthread = new std::thread([&]()
		{
			while (threads_running)
			{
				if (genthread_generating)
				{
					//Generate chunks
					for (auto &i : genthread_in)
					{
						//Break early if thread's supposed to stop
						if (!threads_running)
							break;
						
						//Generate chunk
						GenThread_Out out{
							i,
							{ BlockId_Air },
						};
						chunk_manager->GetChunkGenerator()->GenerateChunk(i, &(out.blocks[0][0][0]));
						genthread_out.push_back(out);
					}
					
					//Stop generating
					genthread_in.clear();
					genthread_generating = false;
				}
				
				//Don't check again for 20ms
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
			}
		});
		
		//Start mesh thread
		meshthread = new std::thread([&]()
		{
			while (threads_running)
			{
				if (meshthread_generating)
				{
					//Generate meshes
					for (auto &i : meshthread_in)
					{
						//Break early if thread's supposed to stop
						if (!threads_running)
							break;
						
						//Generate chunk
						Chunk *chunk = chunk_manager->GetChunk(i);
						if (chunk != nullptr)
						{
							meshthread_out.push_back({
								i,
								chunk->GetMeshData()
							});
						}
					}
					
					//Stop generating
					meshthread_in.clear();
					meshthread_generating = false;
				}
				
				//Don't check again for 20ms
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
			}
		});
		
		return false;
	}
	
	bool World::CheckChunkRange(const ChunkPosition &cam_chunk, const ChunkPosition &chunk_pos)
	{
		auto x_diff = chunk_pos.x - cam_chunk.x;
		auto z_diff = chunk_pos.z - cam_chunk.z;
		if (((x_diff * x_diff) + (z_diff * z_diff)) >= (chunk_range * chunk_range))
			return false;
		return true;
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
		//Kill threads
		threads_running = false;
		if (genthread != nullptr)
		{
			genthread->join();
			delete genthread;
		}
		if (meshthread != nullptr)
		{
			meshthread->join();
			delete meshthread;
		}
		
		//Delete render objects
		delete sky;
		delete terrain_texture;
		delete generic_shader;
		
		//Delete chunk manager
		delete chunk_manager;
	}
	
	//World interface
	bool World::Update(glm::dvec3 cam_pos)
	{
		//Get camera chunk area
		const ChunkPosition &cam_chunk = WorldToChunkPosition(cam_pos);
		
		//Handle chunk generation
		if (!genthread_generating && !meshthread_generating)
		{
			if (genthread_out.size())
			{
				//Create generated chunks
				for (auto &i : genthread_out)
				{
					//Create chunk using data create mesh
					Chunk &chunk = chunk_manager->AddChunk(i.pos);
					chunk.SetData(&(i.blocks[0][0][0]));
					
					//Update meshes
					chunk_mesh_updates[{i.pos.x + 0, i.pos.z + 0}] = true;
					chunk_mesh_updates[{i.pos.x + 1, i.pos.z + 0}] = true; //X+
					chunk_mesh_updates[{i.pos.x + 0, i.pos.z + 1}] = true; //Z+
					chunk_mesh_updates[{i.pos.x - 1, i.pos.z + 0}] = true; //X-
					chunk_mesh_updates[{i.pos.x + 0, i.pos.z - 1}] = true; //Z-
				}
				
				//Clear output array
				genthread_out.clear();
			}
			
			//Queue new chunks
			static const int pos[5][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 0}};
			
			for (int x = 0; x <= chunk_range * 3 / 2; x++)
			{
				//Only queue up to 8 chunks
				if (genthread_in.size() >= 8)
					break;
				
				if (x == 0)
				{
					//Queue chunk that's right at the camera's position
					const ChunkPosition chunk_pos{cam_chunk.x, cam_chunk.z};
					if (!chunk_manager->HasChunk(chunk_pos))
						genthread_in.push_back(chunk_pos);
				}
				else
				{
					//Queue chunks around the camera's position
					for (int i = 0; i < 4; i++)
					{
						for (int v = 0; v < x; v++)
						{
							//Queue chunk
							const ChunkPosition chunk_pos{
								cam_chunk.x + pos[i][0] * (x - v) + pos[i + 1][0] * v,
								cam_chunk.z + pos[i][1] * (x - v) + pos[i + 1][1] * v,
							};
							if (CheckChunkRange(cam_chunk, chunk_pos) && !chunk_manager->HasChunk(chunk_pos))
								genthread_in.push_back(chunk_pos);
						}
					}
				}
			}
			
			//Start generating
			genthread_generating = (genthread_in.size() != 0);
		}
		
		if (!meshthread_generating)
		{
			//Get chunks to unload
			const ChunkPositionMap<Chunk> &chunks = chunk_manager->GetChunks();
			
			std::vector<ChunkPosition> unload_chunks;
			for(auto it = chunks.begin(); it != chunks.end(); it++)
			{
				const ChunkPosition &chunk_pos = it->first;
				if (!CheckChunkRange(cam_chunk, chunk_pos))
					unload_chunks.push_back(chunk_pos);
			}
			
			//Unload chunks
			for (auto &i : unload_chunks)
			{
				//Delete mesh and chunk
				chunk_mesh_updates[i] = false;
				chunk_manager->DeleteChunk(i);
			}
		}
		return false;
	}
	
	bool World::Render(glm::dvec3 cam_pos, glm::dvec3 cam_dir)
	{
		if (!meshthread_generating)
		{
			//Create generated meshes
			while (meshthread_out.size())
			{
				//Get mesh data
				MeshThread_Out &i = meshthread_out.back();
				
				//Make sure chunk still exists
				if (chunk_manager->HasChunk(i.pos))
				{
					//Get chunk mesh object
					auto pos = chunk_meshes.find(i.pos);
					
					ChunkMesh *mesh;
					if (pos == chunk_meshes.cend())
						mesh = &chunk_meshes.emplace(std::piecewise_construct,
													 std::forward_as_tuple(i.pos),
													 std::forward_as_tuple(render)).first->second;
					else
						mesh = &pos->second;
					
					//Send mesh data to chunk mesh object
					if (mesh->GenerateMesh(i.mesh_data))
						return error.Push(mesh->GetError());
				}
				
				//Pop mesh generation
				meshthread_out.pop_back();
			}
			
			if (chunk_mesh_updates.size())
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
							//Update chunk mesh blocks and generate mesh
							chunk->UpdateMeshBlocks();
							meshthread_in.push_back(chunk_pos);
						}
					}
					else
					{
						//Delete chunk mesh at position
						chunk_meshes.erase(chunk_pos);
					}
				}
				
				//Clear mesh updates array
				chunk_mesh_updates.clear();
				
				//Start generating meshes
				meshthread_generating = (meshthread_in.size() != 0);
			}
		}
		
		//Get projection and view matrices
		glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)render->GetWidth() / (float)render->GetHeight(), 0.05f, 16.0f * (chunk_range - 1));
		glm::mat4 view = glm::lookAt({}, cam_dir, {0.0f, 1.0f, 0.0f}); //View remains stationary as the world moves around the origin
		
		//Render sky
		if (sky->Render(projection, view, cam_pos))
			return error.Push(sky->GetError());
		
		//Use generic shader
		generic_shader->Bind();
		generic_shader->SetUniform("u_projection", 1, &(projection[0][0]));
		generic_shader->SetUniform("u_view", 1, &(view[0][0]));
		
		ColourSpace::RGB atmosphere_colour = sky->GetAtmosphereColour();
		generic_shader->SetUniform("u_fog_colour", atmosphere_colour.r, atmosphere_colour.g, atmosphere_colour.b, 1.0f);
		generic_shader->SetUniform("u_fog_start", 8.0f * (chunk_range - 1));
		generic_shader->SetUniform("u_fog_end", 16.0f * (chunk_range - 1));
		
		//Render chunk meshes
		terrain_texture->Bind();
		
		for(auto it = chunk_meshes.begin(); it != chunk_meshes.end(); it++)
		{
			//Get model position
			const ChunkPosition &chunk_pos = it->first;
			glm::mat4 model = glm::translate(glm::mat4(1.0f), {
				-cam_pos.x + (float)chunk_pos.x * CHUNK_DIM,
				-cam_pos.y,
				-cam_pos.z + (float)chunk_pos.z * CHUNK_DIM
			});
			generic_shader->SetUniform("u_model", 1, &(model[0][0]));
			
			//Render mesh
			Backend::Render::Mesh *mesh = (it->second).GetMesh();
			if (mesh != nullptr)
				mesh->Draw();
		}
		
		return false;
	}
}

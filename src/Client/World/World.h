#pragma once
#include <cstdint>
#include <vector>

#include <atomic>
#include <mutex>
#include <thread>

#include "Common/World/WorldDef.h"
#include "Common/World/ChunkManager.h"
#include "ChunkMesh.h"
#include "Sky.h"

#include "../Backend/Render.h"

#include <Common/Util/Error.h>

#include <glm/vec3.hpp>

namespace World
{
	//Generation thread output
	struct GenThread_Out
	{
		ChunkPosition pos;
		Block blocks[CHUNK_HEIGHT][CHUNK_DIM][CHUNK_DIM];
	};
	
	//Mesh thread output
	struct MeshThread_Out
	{
		ChunkPosition pos;
		ChunkMeshData mesh_data;
	};
	
	//World class
	class World
	{
		private:
			//Error
			Error error;
			
			//World information
			int64_t seed;
			
			//Threads
			std::atomic<bool> threads_running = false;
			
			//Chunk state
			ChunkManager *chunk_manager = nullptr;
			
			ChunkPositionMap<ChunkMesh> chunk_meshes;
			ChunkPositionMap<bool> chunk_mesh_updates;
			
			//Render objects
			Backend::Render::Render *render;
			
			Backend::Render::Texture *terrain_texture = nullptr;
			Backend::Render::Shader *generic_shader = nullptr;
			
			Sky *sky = nullptr;
			
			//World generation
			std::atomic<bool> genthread_generating = false;
			std::thread *genthread = nullptr;
			
			std::vector<ChunkPosition> genthread_in;
			std::vector<GenThread_Out> genthread_out;
			
			//Mesh generation
			std::atomic<bool> meshthread_generating = false;
			std::thread *meshthread = nullptr;
			
			std::vector<ChunkPosition> meshthread_in;
			std::vector<MeshThread_Out> meshthread_out;
			
		private:
			//Internal interface
			bool InitWorld();
			
			bool CheckChunkRange(const ChunkPosition &cam_chunk, const ChunkPosition &chunk_pos);
			
		public:
			//Constructors and destructor
			World(Backend::Render::Render *_render);
			World(Backend::Render::Render *_render, int64_t _seed);
			~World();
			
			//World interface
			bool Update(glm::dvec3 cam_pos);
			bool Render(glm::dvec3 cam_pos, glm::dvec3 cam_dir);
			
			//Get seed
			const int64_t &GetSeed() const { return seed; }
			
			//Get chunk manager
			ChunkManager *GetChunkManager() { return chunk_manager; }
			
			//Get error
			const Error &GetError() const { return error; }
	};
}

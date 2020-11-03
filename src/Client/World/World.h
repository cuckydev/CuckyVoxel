#pragma once
#include <cstdint>
#include <vector>

#include "Common/World/WorldDef.h"
#include "Common/World/ChunkManager.h"
#include "ChunkMesh.h"

#include "../Backend/Render.h"

#include <Common/Util/Error.h>

#include <glm/vec3.hpp>

namespace World
{
	//World class
	class World
	{
		private:
			//Error
			Error error;
			
			//World information
			int64_t seed;
			
			//Chunk state
			ChunkManager *chunk_manager = nullptr;
			
			ChunkPositionMap<ChunkMesh> chunk_meshes;
			ChunkPositionMap<bool> chunk_mesh_updates;
			
			//Renderer
			Backend::Render::Render *render;
			Backend::Render::Shader *generic_shader = nullptr;
			
		private:
			//Internal interface
			bool InitWorld();
			
		public:
			//Constructors and destructor
			World(Backend::Render::Render *_render);
			World(Backend::Render::Render *_render, int64_t _seed);
			~World();
			
			//World interface
			bool Update();
			bool Render(glm::vec3 cam_pos, glm::vec3 cam_dir);
			
			//World chunk interface
			void GenerateChunk(const ChunkPosition &chunk_pos);
			
			//Get seed
			const int64_t &GetSeed() const { return seed; }
			
			//Get chunk manager
			ChunkManager *GetChunkManager() { return chunk_manager; }
			
			//Get error
			const Error &GetError() const { return error; }
	};
}

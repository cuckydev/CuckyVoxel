#pragma once
#include <cstdint>
#include "WorldDef.h"
#include "ChunkManager.h"

#include <Common/Util/Error.h>

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
			
			//Chunk manager
			ChunkManager *chunk_manager = nullptr;
			
		private:
			//Internal interface
			bool InitWorld();
			
		public:
			//Constructors and destructor
			World();
			World(int64_t _seed);
			~World();
			
			//World interface
			const int64_t &GetSeed() const { return seed; }
			ChunkManager *GetChunkManager() { return chunk_manager; }
			
			//Get error
			const Error &GetError() const { return error; }
	};
}

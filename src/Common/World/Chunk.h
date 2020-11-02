#pragma once
#include <vector>
#include "WorldDef.h"
#include "WorldPosition.h"
#include "ChunkGenerator.h"

namespace World
{
	class ChunkManager; //prototype
	
	//Chunk types
	struct ChunkVertex
	{
		float x, y, z;
		float r, g, b;
	};
	
	struct ChunkMeshData
	{
		std::vector<ChunkVertex> vert;
		std::vector<unsigned int> ind;
	};
	
	//Chunk class
	class Chunk
	{
		private:
			//Chunk manager and position
			ChunkManager &parent_chunk_manager;
			ChunkPosition pos;
			
			//Chunk block data
			Block blocks[CHUNK_HEIGHT][CHUNK_DIM][CHUNK_DIM] = { BlockId_Air };
			
		public:
			//Constructor and destructor
			Chunk(ChunkManager &_parent_chunk_manager, const ChunkPosition &_pos);
			~Chunk();
			
			//Chunk interface
			void Generate(ChunkGenerator *chunk_generator);
			
			Block GetBlock(const BlockPosition &block_pos) const;
			void SetBlock(const BlockPosition &block_pos, Block block);
			
			ChunkMeshData GetMeshData();
	};
}

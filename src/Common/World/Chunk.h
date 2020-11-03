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
		int u, v;
		BlockId block;
		BlockFace face;
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
			Block mesh_blocks[CHUNK_HEIGHT][CHUNK_DIM][CHUNK_DIM] = { BlockId_Air };
			
		public:
			//Constructor and destructor
			Chunk(ChunkManager &_parent_chunk_manager, const ChunkPosition &_pos);
			~Chunk();
			
			//Chunk interface
			void Generate(ChunkGenerator *chunk_generator);
			void SetData(const Block _blocks[]);
			
			void UpdateMeshBlocks();
			
			BlockId GetBlock(const BlockPosition &block_pos) const;
			void SetBlock(const BlockPosition &block_pos, BlockId block);
			
			BlockId GetBlockImmediate(const BlockPosition &block_pos) const;
			void SetBlockImmediate(const BlockPosition &block_pos, BlockId block);
			
			BlockId GetMeshBlock(const BlockPosition &block_pos);
			
			ChunkMeshData GetMeshData();
	};
}

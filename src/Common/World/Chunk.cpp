#include "Chunk.h"
#include "ChunkManager.h"

#include <algorithm>

#include <Common/Util/Random.h>

namespace World
{
	//Constructor and destructor
	Chunk::Chunk(ChunkManager &_parent_chunk_manager, const ChunkPosition &_pos) : parent_chunk_manager(_parent_chunk_manager), pos(_pos)
	{
		return;
	}
	
	Chunk::~Chunk()
	{
		return;
	}
	
	//Chunk interface
	void Chunk::Generate(ChunkGenerator *chunk_generator)
	{
		chunk_generator->GenerateChunk(pos, &(blocks[0][0][0]));
	}
	
	void Chunk::SetData(const Block _blocks[])
	{
		std::copy_n(&(_blocks[0]), CHUNK_SIZE, &(blocks[0][0][0]));
	}
	
	void Chunk::UpdateMeshBlocks()
	{
		std::copy_n(&(blocks[0][0][0]), CHUNK_SIZE, &(mesh_blocks[0][0][0]));
	}
	
	BlockId Chunk::GetBlock(const BlockPosition &block_pos) const
	{
		if (block_pos.y >= 0 && block_pos.y < CHUNK_HEIGHT)
		{
			if (block_pos.x >= 0 && block_pos.x < CHUNK_DIM && block_pos.z >= 0 && block_pos.z < CHUNK_DIM)
				return (BlockId)blocks[block_pos.y][block_pos.z][block_pos.x];
			else
				return parent_chunk_manager.GetBlock(LocalToWorldBlockPosition(pos, block_pos));
		}
		return BlockId_Air;
	}
	
	void Chunk::SetBlock(const BlockPosition &block_pos, BlockId block)
	{
		if (block_pos.y >= 0 && block_pos.y < CHUNK_HEIGHT)
		{
			if (block_pos.x >= 0 && block_pos.x < CHUNK_DIM && block_pos.z >= 0 && block_pos.z < CHUNK_DIM)
				blocks[block_pos.y][block_pos.z][block_pos.x] = block;
			else
				parent_chunk_manager.SetBlock(LocalToWorldBlockPosition(pos, block_pos), block);
		}
	}
	
	BlockId Chunk::GetBlockImmediate(const BlockPosition &block_pos) const
	{
		return (BlockId)blocks[block_pos.y][block_pos.z][block_pos.x];
	}
	
	void Chunk::SetBlockImmediate(const BlockPosition &block_pos, BlockId block)
	{
		blocks[block_pos.y][block_pos.z][block_pos.x] = block;
	}
	
	BlockId Chunk::GetMeshBlock(const BlockPosition &block_pos)
	{
		return (BlockId)mesh_blocks[block_pos.y][block_pos.z][block_pos.x];
	}
	
	//Mesh generation
	struct ChunkMeshFace
	{
		float vertices[12];
	};
	
	inline void MakeFace(ChunkMeshData &data, int &ind_i, const ChunkMeshFace &mesh_face, const BlockId block_id, const BlockFace block_face, const BlockPosition &pos)
	{
		//Get vertices
		const float *vertex = mesh_face.vertices;
		for (int i = 0; i < 4; i++)
		{
			static const int uv[4][2] = {
				{0, 0},
				{1, 0},
				{1, 1},
				{0, 1},
			};
			data.vert.push_back({
				(float)pos.x + *vertex++,
				(float)pos.y + *vertex++,
				(float)pos.z + *vertex++,
				uv[i][0], uv[i][1],
				block_id,
				block_face,
			});
		}
		
		//Push indices
		data.ind.push_back(ind_i);
		data.ind.push_back(ind_i + 1);
		data.ind.push_back(ind_i + 2);
		data.ind.push_back(ind_i + 2);
		data.ind.push_back(ind_i + 3);
		data.ind.push_back(ind_i);
		ind_i += 4;
	}
	
	inline bool DoMakeFace(BlockId block)
	{
		return block == BlockId_Air;
	}
	
	inline bool DoMakeFace_ChunkEdge(Chunk *chunk, const BlockPosition &pos)
	{
		if (chunk == nullptr)
			return false;
		return DoMakeFace(chunk->GetMeshBlock(pos));
	}
	
	ChunkMeshData Chunk::GetMeshData()
	{
		//Faces
		const ChunkMeshFace front_face =  {{1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1}};
		const ChunkMeshFace right_face =  {{1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0}};
		const ChunkMeshFace back_face =   {{0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0}};
		const ChunkMeshFace left_face =   {{0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1}};
		const ChunkMeshFace top_face =    {{1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1}};
		const ChunkMeshFace bottom_face = {{0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1}};
		
		//Get neighbouring chunks
		Chunk *front_chunk = parent_chunk_manager.GetChunk({pos.x + 0, pos.z + 1});
		Chunk *right_chunk = parent_chunk_manager.GetChunk({pos.x + 1, pos.z + 0});
		Chunk *back_chunk =  parent_chunk_manager.GetChunk({pos.x + 0, pos.z - 1});
		Chunk *left_chunk =  parent_chunk_manager.GetChunk({pos.x - 1, pos.z + 0});
		
		//Mesh data
		ChunkMeshData data;
		int ind_i = 0;
		
		//Generate mesh data
		Block *blockp = &(mesh_blocks[0][0][0]);
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			for (int z = 0; z < CHUNK_DIM; z++)
			{
				for (int x = 0; x < CHUNK_DIM; x++)
				{
					//Get block at position
					const BlockPosition pos{x, y, z};
					BlockId block = (BlockId)(*blockp);
					
					//Process block
					if (block != BlockId_Air)
					{
						//Front
						if ((z == (CHUNK_DIM - 1)) ?
							(DoMakeFace_ChunkEdge(front_chunk, {x, y, 0})) :
							(DoMakeFace((BlockId)*(blockp + CHUNK_DIM))))
							MakeFace(data, ind_i, front_face, block, BlockFace_Front, pos);
						//Back
						if ((z == 0) ?
							(DoMakeFace_ChunkEdge(back_chunk, {x, y, CHUNK_DIM - 1})) :
							(DoMakeFace((BlockId)*(blockp - CHUNK_DIM))))
							MakeFace(data, ind_i, back_face, block, BlockFace_Back, pos);
						//Right
						if ((x == (CHUNK_DIM - 1)) ?
							(DoMakeFace_ChunkEdge(right_chunk, {0, y, z})) :
							(DoMakeFace((BlockId)*(blockp + 1))))
							MakeFace(data, ind_i, right_face, block, BlockFace_Right, pos);
						//Left
						if ((x == 0) ?
							(DoMakeFace_ChunkEdge(left_chunk, {CHUNK_DIM - 1, y, z})) :
							(DoMakeFace((BlockId)*(blockp - 1))))
							MakeFace(data, ind_i, left_face, block, BlockFace_Left, pos);
						//Top
						if ((y == (CHUNK_HEIGHT - 1)) ||
							(DoMakeFace((BlockId)*(blockp + (CHUNK_DIM * CHUNK_DIM)))))
							MakeFace(data, ind_i, top_face, block, BlockFace_Top, pos);
						//Bottom
						if ((y == 0) ||
							(DoMakeFace((BlockId)*(blockp - (CHUNK_DIM * CHUNK_DIM)))))
							MakeFace(data, ind_i, bottom_face, block, BlockFace_Bottom, pos);
					}
					
					//Check next block
					blockp++;
				}
			}
		}
		
		return data;
	}
}

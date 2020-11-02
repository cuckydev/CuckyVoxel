#include "Chunk.h"
#include "World.h"
#include "ChunkManager.h"

#include <Common/Util/Random.h>

#include <iostream>

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
	
	//Chunk terrain generation
	void Chunk::Generate(ChunkGenerator *chunk_generator)
	{
		chunk_generator->GenerateTerrain(pos, &(blocks[0][0][0]));
	}
	
	//Chunk interface
	Block Chunk::GetBlock(const BlockPosition &block_pos) const
	{
		if (block_pos.y >= 0 && block_pos.y < CHUNK_HEIGHT)
		{
			if (block_pos.x >= 0 && block_pos.x < CHUNK_DIM && block_pos.z >= 0 && block_pos.z < CHUNK_DIM)
				return blocks[block_pos.y][block_pos.z][block_pos.x];
			else
				return parent_chunk_manager.GetBlock(LocalToWorldBlockPosition(pos, block_pos));
		}
		return BlockId_Air;
	}
	
	void Chunk::SetBlock(const BlockPosition &block_pos, Block block)
	{
		
	}
	
	//Mesh generation
	struct ChunkMeshFace
	{
		float vertices[12];
		float light;
	};
	
	inline bool DoMakeFace(Chunk &chunk, const BlockPosition &chk_pos)
	{
		Block block = chunk.GetBlock(chk_pos);
		return block == BlockId_Air;
	}
	
	inline void MakeFace(ChunkMeshData &data, int &ind_i, const ChunkMeshFace &face, const BlockPosition &pos)
	{
		//Get vertices
		const float *vertex = face.vertices;
		for (int i = 0; i < 4; i++)
		{
			data.vert.push_back({
				(float)pos.x + *vertex++,
				(float)pos.y + *vertex++,
				(float)pos.z + *vertex++,
				face.light, face.light, face.light
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
	
	ChunkMeshData Chunk::GetMeshData()
	{
		//Faces
		const ChunkMeshFace front_face =  {{1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1}, 0.8f};
		const ChunkMeshFace right_face =  {{1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0}, 0.45f};
		const ChunkMeshFace back_face =   {{0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0}, 0.8f};
		const ChunkMeshFace left_face =   {{0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, 0.45f};
		const ChunkMeshFace top_face =    {{1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1}, 1.0f};
		const ChunkMeshFace bottom_face = {{0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1}, 0.25f};
		
		//Mesh data
		ChunkMeshData data;
		int ind_i = 0;
		
		//Generate mesh data
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			for (int x = 0; x < CHUNK_DIM; x++)
			{
				for (int z = 0; z < CHUNK_DIM; z++)
				{
					const BlockPosition pos{x, y, z};
					bool block = blocks[y][z][x] != BlockId_Air;
					if (block)
					{
						if (DoMakeFace(*this, {x, y, z + 1}))
							MakeFace(data, ind_i, front_face, pos);
						if (DoMakeFace(*this, {x + 1, y, z}))
							MakeFace(data, ind_i, right_face, pos);
						if (DoMakeFace(*this, {x, y, z - 1}))
							MakeFace(data, ind_i, back_face, pos);
						if (DoMakeFace(*this, {x - 1, y, z}))
							MakeFace(data, ind_i, left_face, pos);
						if (DoMakeFace(*this, {x, y + 1, z}))
							MakeFace(data, ind_i, top_face, pos);
						if (DoMakeFace(*this, {x, y - 1, z}))
							MakeFace(data, ind_i, bottom_face, pos);
					}
				}
			}
		}
		
		return data;
	}
}

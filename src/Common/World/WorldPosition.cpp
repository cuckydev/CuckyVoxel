#include "WorldPosition.h"
#include "WorldDef.h"

#include <cmath>

namespace World
{
	ChunkPosition WorldToChunkPosition(const glm::dvec3 &position)
	{
		return BlockToChunkPosition(WorldToBlockPosition(position));
	};
	
	ChunkPosition BlockToChunkPosition(const BlockPosition &position)
	{
		return {
			(int32_t)((position.x < 0) ? ((position.x - CHUNK_DIM) / CHUNK_DIM) : (position.x / CHUNK_DIM)),
			(int32_t)((position.z < 0) ? ((position.z - CHUNK_DIM) / CHUNK_DIM) : (position.z / CHUNK_DIM)),
		};
	};
	
	BlockPosition WorldToLocalBlockPosition(const BlockPosition &position)
	{
		return {
			(CHUNK_DIM + (position.x % CHUNK_DIM)) % CHUNK_DIM,
			position.y,
			(CHUNK_DIM + (position.z % CHUNK_DIM)) % CHUNK_DIM,
		};
	}
	
	BlockPosition LocalToWorldBlockPosition(const ChunkPosition &chunk_position, const BlockPosition &position)
	{
		return {
			(chunk_position.x * CHUNK_DIM) + position.x,
			position.y,
			(chunk_position.z * CHUNK_DIM) + position.z,
		};
	}
	
	BlockPosition WorldToBlockPosition(const glm::dvec3 &position)
	{
		return {
			(int64_t)(std::floor(position.x)),
			(int64_t)(std::floor(position.y)),
			(int64_t)(std::floor(position.z)),
		};
	};
}

#pragma once
#include <cstdint>
#include <glm/vec3.hpp>

namespace World
{
	//Chunk position
	struct ChunkPosition
	{
		//Stored position
		int32_t x, z;
			
		//Hash
		uint64_t Hash() const
		{
			return x * 8976890 + z;
		}
		
		//Operators
		inline friend bool operator==(const ChunkPosition &lhs, const ChunkPosition &rhs)
		{
			return (lhs.x == rhs.x) && (lhs.z == rhs.z);
		}
	};
	
	//Block position
	struct BlockPosition
	{
		int64_t x, y, z;
	};
	
	//Position conversion
	ChunkPosition WorldToChunkPosition(const glm::dvec3 &position);
	ChunkPosition BlockToChunkPosition(const BlockPosition &position);
	BlockPosition WorldToLocalBlockPosition(const BlockPosition &position);
	BlockPosition LocalToWorldBlockPosition(const ChunkPosition &chunk_position, const BlockPosition &position);
	BlockPosition WorldToBlockPosition(const glm::dvec3 &position);
}

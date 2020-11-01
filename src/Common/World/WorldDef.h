#pragma once
#include <cstdint>

//World constants
#define CHUNK_DIM (16)
#define CHUNK_HEIGHT (128)
#define CHUNK_SIZE (CHUNK_DIM * CHUNK_HEIGHT * CHUNK_DIM)

#define WATER_LEVEL (62)
#define CLOUD_LEVEL (127)

namespace World
{
	//World types
	enum BlockId
	{
		BlockId_Air,
		BlockId_Stone,
	};
	
	typedef unsigned char Block;
}

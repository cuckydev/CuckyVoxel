#pragma once
#include <cstdint>

//World constants
#define CHUNK_DIM (16)
#define CHUNK_HEIGHT (128)
#define CHUNK_SIZE (CHUNK_DIM * CHUNK_HEIGHT * CHUNK_DIM)

namespace World
{
	//World types
	enum BlockFace
	{
		BlockFace_Front,
		BlockFace_Right,
		BlockFace_Back,
		BlockFace_Left,
		BlockFace_Top,
		BlockFace_Bottom,
	};
	
	enum BlockId
	{
		BlockId_Air,
		BlockId_Stone,
		BlockId_Grass,
		BlockId_Dirt,
		BlockId_Sand,
		BlockId_Gravel,
		BlockId_Num,
	};
	
	typedef uint8_t Block;
}

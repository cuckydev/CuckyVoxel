#include "World.h"

#include <Common/Util/Random.h>

#include <iostream>

namespace World
{
	//Internal interface
	bool World::InitWorld()
	{
		//Create chunk manager
		delete[] chunk_manager;
		chunk_manager = new ChunkManager(*this);
		if (chunk_manager == nullptr)
			return error.Push("Failed to create chunk manager instance");
		
		return false;
	}
	
	//Constructors and destructor
	World::World()
	{
		//Seed based off time and generate
		seed = RandomTimeSeed();
		InitWorld();
	}
	
	World::World(int64_t _seed)
	{
		//Use given seed and generate
		seed = _seed;
		InitWorld();
	}
	
	World::~World()
	{
		//Delete chunk manager
		delete chunk_manager;
	}
}

#include "ChunkGenerate.h"
#include "World.h"

namespace World
{
	//Constructor and destructor
	ChunkGenerate::ChunkGenerate(World &_parent_world, int64_t seed)
	{
		//Use world and seed
		parent_world = &_parent_world;
		random.SetSeed(seed);
		
		//Initialize noise
		field_912_k.SetOctaves(random, 16);
		field_911_l.SetOctaves(random, 16);
		field_910_m.SetOctaves(random, 8);
		field_909_n.SetOctaves(random, 4);
		field_908_o.SetOctaves(random, 4);
		
		field_922_a.SetOctaves(random, 10);
		field_921_b.SetOctaves(random, 16);
		field_920_c.SetOctaves(random, 8);
	}
	
	ChunkGenerate::~ChunkGenerate()
	{
		
	}
}

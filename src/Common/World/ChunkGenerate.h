#pragma once
#include <cstdint>

#include <Common/Util/Random.h>
#include <Common/Noise/Octaves_Perlin.h>

namespace World
{
	class World; //prototype
	
	//Chunk generation class
	class ChunkGenerate
	{
		private:
			//Parent world and random
			World *parent_world = nullptr;
			Random random;
			
			//Noise generators
			Noise::Octaves_Perlin field_912_k;
			Noise::Octaves_Perlin field_911_l;
			Noise::Octaves_Perlin field_910_m;
			Noise::Octaves_Perlin field_909_n;
			Noise::Octaves_Perlin field_908_o;
			
		public:
			//Noise generators
			Noise::Octaves_Perlin field_922_a;
			Noise::Octaves_Perlin field_921_b;
			Noise::Octaves_Perlin field_920_c;
			
		public:
			//Constructor and destructor
			ChunkGenerate(World &_parent_world, int64_t seed);
			~ChunkGenerate();
	};
}

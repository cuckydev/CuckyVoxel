#pragma once
#include "WorldDef.h"
#include "WorldPosition.h"

#include <Common/Noise/Octaves_Perlin.h>

namespace World
{
	class ChunkManager; //prototype
	
	//Chunk generator class
	class ChunkGenerator
	{
		private:
			//Chunk manager
			ChunkManager &parent_chunk_manager;
			int64_t seed;
			
			//Terrain noise
			Noise::Octaves_Perlin field_912_k;
			Noise::Octaves_Perlin field_911_l;
			Noise::Octaves_Perlin field_910_m;
			Noise::Octaves_Perlin field_909_n;
			Noise::Octaves_Perlin field_908_o;
			
			Noise::Octaves_Perlin field_922_a;
			Noise::Octaves_Perlin field_921_b;
			Noise::Octaves_Perlin field_920_c;
			
		private:
			//Internal interface
			void InitializeNoiseField(double out[], int x, int y, int z, double temperature[], double humidity[]);
			
		public:
			//Constructor and destructor
			ChunkGenerator(ChunkManager &_parent_chunk_manager);
			~ChunkGenerator();
			
			//Chunk generator interface
			void GenerateTerrain(const ChunkPosition &pos, Block blocks[]);
	};
}

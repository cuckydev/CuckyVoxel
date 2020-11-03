#pragma once
#include "WorldDef.h"
#include "WorldPosition.h"

#include <Common/Noise/Octaves_Perlin.h>
#include <Common/Util/Random.h>

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
			
			//Random and noise
			Random random;
			
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
			void GenerateTerrain(const ChunkPosition &pos, Block blocks[], double temperature[], double humidity[]);
			void GenerateSurface(const ChunkPosition &pos, Block blocks[]);
			
		public:
			//Constructor and destructor
			ChunkGenerator(ChunkManager &_parent_chunk_manager);
			~ChunkGenerator();
			
			//Chunk generator interface
			void GenerateChunk(const ChunkPosition &pos, Block blocks[]);
	};
}

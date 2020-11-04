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
			
			//Random and noise generators
			Random random;
			
			Noise::Octaves_Perlin noise_2d_1;
			Noise::Octaves_Perlin noise_2d_2;
			
			Noise::Octaves_Perlin noise_3d_1;
			Noise::Octaves_Perlin noise_3d_2;
			Noise::Octaves_Perlin noise_3d_lerp;
			
			Noise::Octaves_Perlin noise_surf_map;
			Noise::Octaves_Perlin noise_surf_depth;
			
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

#pragma once
#include <cstdint>
#include <unordered_map>
#include "WorldDef.h"
#include "WorldPosition.h"
#include "Chunk.h"

#include <Common/Noise/Octaves_Perlin.h>
#include <Common/Noise/Octaves_Simplex.h>

namespace World
{
	class World; //prototype
	
	//Chunk position mapping
	struct ChunkPositionHash
	{
		std::size_t operator()(const ChunkPosition& position) const
		{
			return position.Hash();
		}
	};
	
	template <typename T>
	using ChunkPositionMap = std::unordered_map<ChunkPosition, T, ChunkPositionHash>;
	
	//Chunk manager
	class ChunkManager
	{
		private:
			//Parent world
			World &parent_world;
			int64_t seed;
			
			//Current chunks
			ChunkPositionMap<Chunk> chunks;
			
			//Noise maps
			Noise::Octaves_Simplex temperature_noise;
			Noise::Octaves_Simplex humidity_noise;
			Noise::Octaves_Simplex biome_noise;
			
		public:
			//Constructor and destructor
			ChunkManager(World &parent_world);
			~ChunkManager();
			
			//Chunk generation interface
			void GetChunkWeather(double temperature[], double humidity[], const ChunkPosition &chunk_pos);
			
			//Chunk manager interface
			Chunk &CreateChunk(const ChunkPosition &chunk_pos);
			void DeleteChunk(const ChunkPosition &chunk_pos);
			const Chunk &GetChunk(const ChunkPosition &chunk_pos);
			
			const int64_t &GetSeed() const { return seed; }
	};
}

#pragma once
#include <cstdint>
#include <unordered_map>
#include "WorldDef.h"
#include "WorldPosition.h"
#include "Chunk.h"
#include "ChunkGenerator.h"

#include <Common/Util/Error.h>

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
			//Error
			Error error;
			
			//Parent world
			World &parent_world;
			int64_t seed;
			
			//Current chunks
			ChunkPositionMap<Chunk> chunks;
			
			//Chunk generator
			ChunkGenerator *chunk_generator;
			
			//Weather noise
			Noise::Octaves_Simplex temperature_noise;
			Noise::Octaves_Simplex humidity_noise;
			Noise::Octaves_Simplex biome_noise;
			
		public:
			//Constructor and destructor
			ChunkManager(World &parent_world);
			~ChunkManager();
			
			//Chunk generation interface
			void GetChunkWeather(const ChunkPosition &chunk_pos, double temperature[], double humidity[]);
			
			//Chunk manager interface
			Chunk &CreateChunk(const ChunkPosition &chunk_pos);
			Chunk &GenerateChunk(const ChunkPosition &chunk_pos);
			void DeleteChunk(const ChunkPosition &chunk_pos);
			const Chunk *GetChunk(const ChunkPosition &chunk_pos);
			
			Block GetBlock(const BlockPosition &pos);
			
			const int64_t &GetSeed() const { return seed; }
			
			//Get error
			const Error &GetError() const { return error; }
	};
}

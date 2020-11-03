#pragma once
#include <cstdint>
#include <unordered_map>
#include <vector>

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
			
			//Seed
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
			ChunkManager(int64_t _seed);
			~ChunkManager();
			
			//Chunk generation interface
			void GetChunkWeather(const ChunkPosition &chunk_pos, double temperature[], double humidity[]);
			
			//Chunk interface
			bool HasChunk(const ChunkPosition &chunk_pos) const;
			Chunk *GetChunk(const ChunkPosition &chunk_pos);
			
			Chunk &AddChunk(const ChunkPosition &chunk_pos);
			void DeleteChunk(const ChunkPosition &chunk_pos);
			
			Chunk &GenerateChunk(const ChunkPosition &chunk_pos);
			
			bool HasNeighbours(const ChunkPosition &chunk_pos) const;
			void AddNeighbours(const ChunkPosition &chunk_pos);
			
			const ChunkPositionMap<Chunk> &GetChunks() const { return chunks; }
			
			//Block interface
			BlockId GetBlock(const BlockPosition &pos);
			void SetBlock(const BlockPosition &pos, BlockId block);
			
			//Get seed
			const int64_t &GetSeed() const { return seed; }
			
			//Get chunk generator
			ChunkGenerator *GetChunkGenerator() { return chunk_generator; }
			
			//Get error
			const Error &GetError() const { return error; }
	};
}

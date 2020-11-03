#include "ChunkManager.h"

namespace World
{
	//Constructor and destructor
	ChunkManager::ChunkManager(int64_t _seed) : seed(_seed)
	{
		//Create chunk generator
		chunk_generator = new ChunkGenerator(*this);
		if (chunk_generator == nullptr)
		{
			error.Push("Failed to create chunk manager instance");
			return;
		}
		
		//Initialize weather noise
		temperature_noise.SetOctaves(seed * 9871, 4);
		humidity_noise.SetOctaves(seed * 398811, 4);
		biome_noise.SetOctaves(seed * 543321, 2);
	}
	
	ChunkManager::~ChunkManager()
	{
		//Delete chunk generator
		delete chunk_generator;
	}
	
	//Chunk generation interface
	void ChunkManager::GetChunkBiome(const ChunkPosition &chunk_pos, double temperature[], double humidity[])
	{
		//Get temperature and humidity noise
		temperature_noise.Noise(temperature, chunk_pos.x * CHUNK_DIM, chunk_pos.z * CHUNK_DIM, CHUNK_DIM, CHUNK_DIM, 0.025, 0.025, 0.25);
		humidity_noise.Noise(humidity,       chunk_pos.x * CHUNK_DIM, chunk_pos.z * CHUNK_DIM, CHUNK_DIM, CHUNK_DIM, 0.05,  0.05,  0.3333333333333333);
		
		//Get biome noise
		double biome[CHUNK_DIM * CHUNK_DIM] = {};
		biome_noise.Noise(biome, chunk_pos.x * CHUNK_DIM, chunk_pos.z * CHUNK_DIM, CHUNK_DIM, CHUNK_DIM, 0.25, 0.25, 0.5882352941176471);
		
		//Calculate final temperature and noise based off biome noise
		double *tempp = temperature, *humidp = humidity, *biomep = biome;
		for (int x = 0; x < CHUNK_DIM; x++)
		{
			for (int z = 0; z < CHUNK_DIM; z++)
			{
				//Get biome factor
				const double biome_factor = *biomep * 1.1 + 0.5;
				
				//Get next temperature and humidity
				double next_temp = ((*tempp * 0.15 + 0.7) * (1.0 - 0.01)) + (biome_factor * 0.01);
				if (next_temp < 0.0)
					next_temp = 0.0;
				if (next_temp > 1.0)
					next_temp = 1.0;
				
				double next_humid = ((*humidp * 0.15 + 0.5) * (1.0 - 0.002)) + (biome_factor * 0.002);
				if (next_humid < 0.0)
					next_humid = 0.0;
				if (next_humid > 1.0)
					next_humid = 1.0;
				
				//Apply weather to array and calculate next
				*tempp++ = next_temp;
				*humidp++ = next_humid;
				biomep++;
			}
		}
	}
	
	//Chunk manager interface
	bool ChunkManager::HasChunk(const ChunkPosition &chunk_pos) const
	{
		return chunks.find(chunk_pos) != chunks.cend();
	}
	
	Chunk *ChunkManager::GetChunk(const ChunkPosition &chunk_pos)
	{
		//Return chunk at given position or empty placeholder
		auto pos = chunks.find(chunk_pos);
		if (pos == chunks.cend())
			return nullptr;
		return &pos->second;
	}
	
	Chunk &ChunkManager::AddChunk(const ChunkPosition &chunk_pos)
	{
		//Allocate a new chunk at the given position
		auto pos = chunks.find(chunk_pos);
		if (pos == chunks.cend())
			return chunks.emplace(std::piecewise_construct,
			                      std::forward_as_tuple(chunk_pos),
			                      std::forward_as_tuple(*this, chunk_pos)).first->second;
		return pos->second;
	}
	
	void ChunkManager::DeleteChunk(const ChunkPosition &chunk_pos)
	{
		//Delete chunk at the given position
		chunks.erase(chunk_pos);
	}
	
	Chunk &ChunkManager::GenerateChunk(const ChunkPosition &chunk_pos)
	{
		Chunk &chunk = AddChunk(chunk_pos);
		chunk.Generate(chunk_generator);
		return chunk;
	}
	
	bool ChunkManager::HasNeighbours(const ChunkPosition &chunk_pos) const
	{
		return HasChunk({chunk_pos.x + 0, chunk_pos.z + 0}) &&
		       HasChunk({chunk_pos.x + 1, chunk_pos.z + 0}) && //+X
		       HasChunk({chunk_pos.x + 0, chunk_pos.z + 1}) && //+Z
		       HasChunk({chunk_pos.x - 1, chunk_pos.z + 0}) && //-X
		       HasChunk({chunk_pos.x + 0, chunk_pos.z - 1});   //-Z
	}
	
	void ChunkManager::AddNeighbours(const ChunkPosition &chunk_pos)
	{
		AddChunk({chunk_pos.x + 0, chunk_pos.z + 0});
		AddChunk({chunk_pos.x + 1, chunk_pos.z + 0}); //+X
		AddChunk({chunk_pos.x + 0, chunk_pos.z + 1}); //+Z
		AddChunk({chunk_pos.x - 1, chunk_pos.z + 0}); //-X
		AddChunk({chunk_pos.x + 0, chunk_pos.z - 1}); //-Z
	}
	
	//Block interface
	BlockId ChunkManager::GetBlock(const BlockPosition &pos)
	{
		Chunk *containing_chunk = GetChunk(BlockToChunkPosition(pos));
		if (containing_chunk == nullptr)
			return BlockId_Air;
		return containing_chunk->GetBlockImmediate(WorldToLocalBlockPosition(pos));
	}
	
	void ChunkManager::SetBlock(const BlockPosition &pos, BlockId block)
	{
		Chunk *containing_chunk = GetChunk(BlockToChunkPosition(pos));
		if (containing_chunk != nullptr)
			containing_chunk->SetBlockImmediate(WorldToLocalBlockPosition(pos), block);
	}
}

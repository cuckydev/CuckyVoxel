#include "ChunkManager.h"
#include "World.h"

namespace World
{
	//Constructor and destructor
	ChunkManager::ChunkManager(World &_parent_world) : parent_world(_parent_world)
	{
		//Use parent world information
		seed = parent_world.GetSeed();
		
		//Initialize noise
		temperature_noise.SetOctaves(seed * 9871, 4);
		humidity_noise.SetOctaves(seed * 398811, 4);
		biome_noise.SetOctaves(seed * 543321, 2);
	}
	
	ChunkManager::~ChunkManager()
	{
		
	}
	
	//Chunk generation interface
	void ChunkManager::GetChunkWeather(double temperature[], double humidity[], const ChunkPosition &chunk_pos)
	{
		//Get temperature and humidity noise
		temperature_noise.Noise(temperature, chunk_pos.x * CHUNK_DIM, chunk_pos.z * CHUNK_DIM, CHUNK_DIM, CHUNK_DIM, 0.025, 0.025, 0.25);
		humidity_noise.Noise(humidity,       chunk_pos.x * CHUNK_DIM, chunk_pos.z * CHUNK_DIM, CHUNK_DIM, CHUNK_DIM, 0.05,  0.05,  0.3333333333333333);
		
		//Get biome noise
		double biome[CHUNK_DIM * CHUNK_DIM];
		biome_noise.Noise(biome, chunk_pos.x * CHUNK_DIM, chunk_pos.z * CHUNK_DIM, CHUNK_DIM, CHUNK_DIM, 0.25,  0.25,  0.5882352941176471);
		
		//Calculate final temperature and noise based off biome noise
		double *tempp = temperature, *humidp = humidity, *biomep = biome;
		for (int z = 0; z < CHUNK_DIM; z++)
		{
			for (int x = 0; x < CHUNK_DIM; x++)
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
	Chunk &ChunkManager::CreateChunk(const ChunkPosition &chunk_pos)
	{
		//Allocate a new chunk at the given position
		auto pos = chunks.find(chunk_pos);
		if (pos == chunks.cend())
			return chunks.emplace(std::piecewise_construct, std::forward_as_tuple(chunk_pos), std::forward_as_tuple(*this, chunk_pos)).first->second;
		return pos->second;
	}
	
	void ChunkManager::DeleteChunk(const ChunkPosition &chunk_pos)
	{
		//Delete chunk at the given position
		chunks.erase(chunk_pos);
	}
	
	const Chunk &ChunkManager::GetChunk(const ChunkPosition &chunk_pos)
	{
		//Return chunk at given position or empty placeholder
		auto pos = chunks.find(chunk_pos);
		if (pos == chunks.cend())
		{
			static Chunk placeholder_chunk(*this, {0, 0});
			return placeholder_chunk;
		}
		return pos->second;
	}
}

#include "ChunkGenerator.h"
#include "ChunkManager.h"

namespace World
{
	//Terrain generate constants
	static const int noise_xp = 4;
	static const int noise_yp = 8;
	
	static const int noise_dim = (CHUNK_DIM + (noise_xp - 1)) / noise_xp;
	static const int noise_height = (CHUNK_HEIGHT + (noise_yp - 1)) / noise_yp;
	static const int water_level = 64;
	
	static const int noise_xd = noise_dim + 1;
	static const int noise_yd = noise_height + 1;
	static const int noise_zd = noise_dim + 1;
	
	//Internal interface
	void ChunkGenerator::InitializeNoiseField(double out[], int x, int y, int z, double temperature[], double humidity[])
	{
		//Constants
		static const double xf = 684.412;
		static const double yf = 684.412;
		
		//Generate noise tables
		double layer_2d_1[noise_xd][noise_zd] = {};
		double layer_2d_2[noise_xd][noise_zd] = {};
		noise_2d_1.Noise(&(layer_2d_1[0][0]), x, z, noise_xd, noise_zd, 1.121, 1.121);
		noise_2d_2.Noise(&(layer_2d_2[0][0]), x, z, noise_xd, noise_zd, 200.0, 200.0);
		
		double layer_3d_lerp[noise_xd][noise_zd][noise_yd] = {};
		double layer_3d_1[noise_xd][noise_zd][noise_yd] = {};
		double layer_3d_2[noise_xd][noise_zd][noise_yd] = {};
		noise_3d_lerp.Noise(&(layer_3d_lerp[0][0][0]), x, y, z, noise_xd, noise_yd, noise_zd, xf / 80.0, yf / 160.0, xf / 80.0);
		noise_3d_1.Noise(&(layer_3d_1[0][0][0]),       x, y, z, noise_xd, noise_yd, noise_zd, xf,        yf,         xf);
		noise_3d_2.Noise(&(layer_3d_2[0][0][0]),       x, y, z, noise_xd, noise_yd, noise_zd, xf,        yf,         xf);
		
		//Process noise
		double *layer_2d_1_p = &(layer_2d_1[0][0]);
		double *layer_2d_2_p = &(layer_2d_2[0][0]);
		
		double *layer_3d_lerp_p = &(layer_3d_lerp[0][0][0]);
		double *layer_3d_1_p = &(layer_3d_1[0][0][0]);
		double *layer_3d_2_p = &(layer_3d_2[0][0][0]);
		
		double *temp_p = temperature;
		double *humid_p = humidity;
		
		for (int nx = 0; nx < noise_xd; nx++)
		{
			const int px = nx * (CHUNK_DIM - 1) / noise_xd;
			for (int nz = 0; nz < noise_zd; nz++)
			{
				const int pz = nz * (CHUNK_DIM - 1) / noise_zd;
				
				//Handle biome at point
				const double point_temp = temperature[px * CHUNK_DIM + pz];
				const double point_humid =   humidity[px * CHUNK_DIM + pz] * point_temp;
				
				double humid_factor = 1.0 - point_humid;
				humid_factor *= humid_factor;
				humid_factor *= humid_factor;
				humid_factor = 1.0 - humid_factor;
				
				//Get 2D layer values
				double value_2d_1 = (((*layer_2d_1_p++) + 256.0) / 512.0) * humid_factor;
				if (value_2d_1 > 1.0)
					value_2d_1 = 1.0;
				
				double value_2d_2 = (*layer_2d_2_p++) / 8000.0;
				if (value_2d_2 < 0.0)
					value_2d_2 *= -0.3;
				value_2d_2 = (value_2d_2 * 3.0) - 2.0;
				
				if (value_2d_2 < 0.0)
				{
					value_2d_2 /= 2.0;
					if (value_2d_2 < -1.0)
						value_2d_2 = -1.0;
					value_2d_2 /= 1.4;
					value_2d_2 /= 2.0;
					value_2d_1 = 0.0;
				}
				else
				{
					if (value_2d_2 > 1.0)
						value_2d_2 = 1.0;
					value_2d_2 /= 8.0;
				}
				
				if(value_2d_1 < 0.0)
					value_2d_1 = 0.0;
				value_2d_1 += 0.5;
				
				//Get final height, brought up to the water level
				static const int water_yd = (water_level * 2 / noise_yp) + 1;
				
				value_2d_2 *= (double)water_yd / 16.0;
				const double height = ((double)water_yd / 2.0) + (value_2d_2 * 4.0);
				
				for (int ny = 0; ny < noise_yd; ny++)
				{
					//Get height mod
					double final_value = 0.0;
					double height_mod = (((double)ny - height) * 12.0) / value_2d_1;
					if (height_mod < 0.0)
						height_mod *= 4.0; //Flatten terrain below water level
					
					//Apply 3D noise layers to final value
					const double value_3d_1 = ((*layer_3d_1_p++) / 512.0);
					const double value_3d_2 = ((*layer_3d_2_p++) / 512.0);
					const double value_3d_lerp = ((*layer_3d_lerp_p++) / 10.0 + 1.0) / 2.0;
					
					if (value_3d_lerp < 0.0D)
						final_value = value_3d_1;
					else if(value_3d_lerp > 1.0D)
						final_value = value_3d_2;
					else
						final_value = value_3d_1 + (value_3d_2 - value_3d_1) * value_3d_lerp;
					final_value -= height_mod;
					
					//Flatten terrain near the top of the world
					if (ny > (noise_yd - 4))
					{
						const double lerp = (double)(ny - (noise_yd - 4)) / 3.0;
						final_value = (final_value * (1.0 - lerp)) + (-10.0 * lerp);
					}
					
					//Set noise value
					*out++ = final_value;
				}
			}
		}
	}
	
	void ChunkGenerator::GenerateTerrain(const ChunkPosition &pos, Block blocks[], double temperature[], double humidity[])
	{
		//Get noise
		double noise[noise_xd][noise_zd][noise_yd] = {};
		InitializeNoiseField(&(noise[0][0][0]), pos.x * noise_dim, 0, pos.z * noise_dim, temperature, humidity);
		
		//Process noise points
		for (int nx = 0; nx < noise_dim; nx++)
		{
			for (int nz = 0; nz < noise_dim; nz++)
			{
				for (int ny = 0; ny < noise_height; ny++)
				{
					//Get noise chunk points
					const double c000 = noise[nx + 0][nz + 0][ny + 0];
					const double c100 = noise[nx + 1][nz + 0][ny + 0];
					const double c010 = noise[nx + 0][nz + 1][ny + 0];
					const double c110 = noise[nx + 1][nz + 1][ny + 0];
					
					const double c001 = noise[nx + 0][nz + 0][ny + 1];
					const double c101 = noise[nx + 1][nz + 0][ny + 1];
					const double c011 = noise[nx + 0][nz + 1][ny + 1];
					const double c111 = noise[nx + 1][nz + 1][ny + 1];
					
					//Process noise chunk as block data using trilinear interpolation
					static const int zinc = CHUNK_DIM - noise_xp;
					static const int yinc = zinc * CHUNK_DIM;
					Block *blockp = &blocks[((ny * noise_yp) * (CHUNK_DIM * CHUNK_DIM)) + ((nz * noise_xp) * CHUNK_DIM) + (nx * noise_xp)];
					
					double v00x = c000;
					double v10x = c100;
					double v01x = c010;
					double v11x = c110;
					
					const double i00x = (c001 - c000) / (double)noise_yp;
					const double i10x = (c101 - c100) / (double)noise_yp;
					const double i01x = (c011 - c010) / (double)noise_yp;
					const double i11x = (c111 - c110) / (double)noise_yp;
					
					for (int sy = 0; sy < noise_yp; sy++)
					{
						double vx0 = v00x;
						double v1x = v10x;
						double ix0 = (v01x - v00x) / (double)noise_xp;
						double i1x = (v11x - v10x) / (double)noise_xp;
						
						for (int sz = 0; sz < noise_xp; sz++)
						{
							double v = vx0;
							double i = (v1x - vx0) / (double)noise_xp;
							
							for (int sx = 0; sx < noise_xp; sx++)
							{
								//int bx = (nx * noise_xp) + sx;
								//int by = (ny * noise_yp) + sy;
								//int bz = (nz * noise_xp) + sz;
								
								//Get block to place
								BlockId next_block;
								if (v > 0.0)
									next_block = BlockId_Stone;
								else
									next_block = BlockId_Air;
								
								//Place block
								*blockp++ = next_block;
								
								v += i;
							}
							
							vx0 += ix0;
							v1x += i1x;
							
							blockp += zinc;
						}
						
						v00x += i00x;
						v10x += i10x;
						v01x += i01x;
						v11x += i11x;
						
						blockp += yinc;
					}
				}
			}
		}
	}

	
	void ChunkGenerator::GenerateSurface(const ChunkPosition &pos, Block blocks[])
	{
		//Get noise
		double gravel_map[CHUNK_DIM * CHUNK_DIM] = {};
		double sand_map[CHUNK_DIM * CHUNK_DIM] = {};
		double depth_map[CHUNK_DIM * CHUNK_DIM] = {};
		noise_surf_map.Noise(gravel_map, pos.x * CHUNK_DIM, 109.0134, pos.z * CHUNK_DIM, CHUNK_DIM, 1, CHUNK_DIM, 0.03125, 1.0, 0.03125);
		noise_surf_map.Noise(sand_map, pos.x * CHUNK_DIM, pos.z * CHUNK_DIM, CHUNK_DIM, CHUNK_DIM, 0.03125, 0.03125);
		noise_surf_depth.Noise(depth_map, pos.x * CHUNK_DIM, pos.z * CHUNK_DIM, CHUNK_DIM, CHUNK_DIM, 0.03125 * 2.0, 0.03125 * 2.0);
		
		//Generate surface
		double *gravel_map_p = gravel_map;
		double *sand_map_p = sand_map;
		double *depth_map_p = depth_map;
		
		for (int x = 0; x < CHUNK_DIM; x++)
		{
			for (int z = 0; z < CHUNK_DIM; z++)
			{
				Block *blockp = &blocks[(CHUNK_HEIGHT * (CHUNK_DIM * CHUNK_DIM)) + (z * CHUNK_DIM) + (x)];
				
				int surf_i = -1;
				
				BlockId top_block = BlockId_Grass;
				BlockId filler_block = BlockId_Dirt;
				
				bool surf_sand = (*sand_map_p++ + random.NextDouble() * 0.2) > 0.0;
				bool surf_gravel = (*gravel_map_p++ + random.NextDouble() * 0.2) > 3.0;
				int surf_depth = (int)(*depth_map_p++ / 3.0 + 3.0 + random.NextDouble() * 0.25);
				
				for (int y = (CHUNK_HEIGHT - 1); y >= 0; y--)
				{
					//Shift down 1 block
					blockp -= CHUNK_DIM * CHUNK_DIM;
					
					//Get block and make sure it's a surface block
					BlockId block = (BlockId)(*blockp);
					if(block == BlockId_Air)
					{
						surf_i = -1;
						continue;
					}
					if(block != BlockId_Stone)
						continue;
					
					//Get top and filler block
					if (surf_i == -1)
					{
						if (surf_depth <= 0)
						{
							top_block = BlockId_Air;
							filler_block = BlockId_Stone;
						}
						else if (y >= water_level - 4 && y <= water_level + 1)
						{
							if (surf_sand)
							{
								top_block = BlockId_Sand;
								filler_block = BlockId_Sand;
							}
							else if (surf_gravel)
							{
								top_block = BlockId_Air;
								filler_block = BlockId_Gravel;
							}
							else
							{
								top_block = BlockId_Grass;
								filler_block = BlockId_Dirt;
							}
						}
						
						//if(y < water_level && top_block == BlockId_Air)
						//	top_block = BlockId_FlowingWater;
						
						//Set top block
						surf_i = surf_depth;
						if (y >= water_level - 1)
							*blockp = top_block;
						else
							*blockp = filler_block;
					}
					else
					{
						//Set filler block
						if (surf_i > 0)
						{
							*blockp = filler_block;
							surf_i--;
						}
					}
				}
			}
		}
	}
	
	//Constructor and destructor
	ChunkGenerator::ChunkGenerator(ChunkManager &_parent_chunk_manager) : parent_chunk_manager(_parent_chunk_manager)
	{
		//Use parent chunk manager
		seed = parent_chunk_manager.GetSeed();
		
		//Initialize noise generators
		noise_2d_1.SetOctaves(seed, 10);
		noise_2d_2.SetOctaves(seed, 16);
		
		noise_3d_1.SetOctaves(seed, 16);
		noise_3d_2.SetOctaves(seed, 16);
		noise_3d_lerp.SetOctaves(seed, 8);
		
		noise_surf_map.SetOctaves(seed, 4);
		noise_surf_depth.SetOctaves(seed, 4);
	}
	
	ChunkGenerator::~ChunkGenerator()
	{
		return;
	}
	
	//Chunk generator interface
	void ChunkGenerator::GenerateChunk(const ChunkPosition &pos, Block blocks[])
	{
		//Seed random off chunk position
		random.SetSeed((0x4F9939F508 * pos.x) + (0x1EF1565BD5 * pos.z));
		
		//Get biome tables
		double temperature[CHUNK_DIM][CHUNK_DIM] = {};
		double humidity[CHUNK_DIM][CHUNK_DIM] = {};
		parent_chunk_manager.GetChunkBiome(pos, &(temperature[0][0]), &(humidity[0][0]));
		
		//Generate chunk step by step
		GenerateTerrain(pos, blocks, &(temperature[0][0]), &(humidity[0][0]));
		GenerateSurface(pos, blocks);
	}
}

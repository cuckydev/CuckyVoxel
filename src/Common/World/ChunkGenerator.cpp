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
		double layer_2d_1[noise_xd * noise_zd] = {};
		double layer_2d_2[noise_xd * noise_zd] = {};
		field_922_a.Noise(layer_2d_1, x, z, noise_xd, noise_zd, 1.121, 1.121);
		field_921_b.Noise(layer_2d_2, x, z, noise_xd, noise_zd, 200.0, 200.0);
		
		double layer_3d_1[noise_xd * noise_zd * noise_yd] = {};
		double layer_3d_2[noise_xd * noise_zd * noise_yd] = {};
		double layer_3d_3[noise_xd * noise_zd * noise_yd] = {};
		field_910_m.Noise(layer_3d_1, x, y, z, noise_xd, noise_yd, noise_zd, xf / 80.0, yf / 160.0, xf / 80.0);
		field_912_k.Noise(layer_3d_2, x, y, z, noise_xd, noise_yd, noise_zd, xf,        yf,         xf);
		field_911_l.Noise(layer_3d_3, x, y, z, noise_xd, noise_yd, noise_zd, xf,        yf,         xf);
		
		//Process noise
		int p_2d = 0;
		int p_3d = 0;
		
		for (int nx = 0; nx < noise_xd; nx++)
		{
			const int px = nx * (CHUNK_DIM - 1) / noise_xd;
			for (int nz = 0; nz < noise_zd; nz++)
			{
				const int pz = nz * (CHUNK_DIM - 1) / noise_zd;
				
				//Handle weather at point
				const double point_temp = temperature[px * CHUNK_DIM + pz];
				const double point_humid =   humidity[px * CHUNK_DIM + pz] * point_temp;
				
				double humid_factor = 1.0 - point_humid;
				humid_factor *= humid_factor;
				humid_factor *= humid_factor;
				humid_factor = 1.0 - humid_factor;
				
				//Handle 2D layers
				double value_2d_1 = ((layer_2d_1[p_2d] + 256.0) / 512.0) * humid_factor;
				if (value_2d_1 > 1.0)
					value_2d_1 = 1.0;
				
				double value_2d_2 = layer_2d_2[p_2d] / 8000.0;
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
				
				//Get final 2D layer value
				value_2d_2 *= (double)noise_yd / 16.0;
				const double d7 = ((double)noise_yd / 2.0) + (value_2d_2 * 4.0);
				p_2d++;
				
				for (int ny = 0; ny < noise_yd; ny++)
				{
					//Handle 3D layers
					double d8 = 0.0;
					double d9 = (((double)ny - d7) * 12.0) / value_2d_1;
					if (d9 < 0.0)
						d9 *= 4.0;
					
					const double value_3d_2 = (layer_3d_2[p_3d] / 512.0);
					const double value_3d_3 = (layer_3d_3[p_3d] / 512.0);
					const double value_3d_1 = (layer_3d_1[p_3d] / 10.0 + 1.0) / 2.0;
					
					if (value_3d_1 < 0.0D)
						d8 = value_3d_2;
					else if(value_3d_1 > 1.0D)
						d8 = value_3d_3;
					else
						d8 = value_3d_2 + (value_3d_3 - value_3d_2) * value_3d_1;
					d8 -= d9;
					
					if (ny > (noise_yd - 4))
					{
						const double d13 = (double)(ny - (noise_yd - 4)) / 3.0;
						d8 = (d8 * (1.0 - d13)) + (-10.0 * d13);
					}
					
					//Set noise
					out[p_3d] = d8;
					p_3d++;
				}
			}
		}
	}
	
	void ChunkGenerator::GenerateTerrain(const ChunkPosition &pos, Block blocks[], double temperature[], double humidity[])
	{
		//Get noise
		double noise[noise_xd][noise_zd][noise_yd] = {};
		InitializeNoiseField(&(noise[0][0][0]), pos.x * noise_dim, 0, pos.z * noise_dim, temperature, humidity);
		
		for (int ny = 0; ny < noise_height; ny++)
		{
			for (int nz = 0; nz < noise_dim; nz++)
			{
				for (int nx = 0; nx < noise_dim; nx++)
				{
					//Get noise points
					double d1 = (noise[nx + 0][nz + 0][ny + 0]);
					double d2 = (noise[nx + 1][nz + 0][ny + 0]);
					double d3 = (noise[nx + 0][nz + 1][ny + 0]);
					double d4 = (noise[nx + 1][nz + 1][ny + 0]);
					
					double d5 = (noise[nx + 0][nz + 0][ny + 1] - d1) / (double)noise_yp;
					double d6 = (noise[nx + 1][nz + 0][ny + 1] - d2) / (double)noise_yp;
					double d7 = (noise[nx + 0][nz + 1][ny + 1] - d3) / (double)noise_yp;
					double d8 = (noise[nx + 1][nz + 1][ny + 1] - d4) / (double)noise_yp;
					
					//Process noise as block data
					static const int zinc = CHUNK_DIM - noise_xp;
					static const int yinc = zinc * CHUNK_DIM;
					Block *blockp = &blocks[((ny * noise_yp) * (CHUNK_DIM * CHUNK_DIM)) + ((nz * noise_xp) * CHUNK_DIM) + (nx * noise_xp)];
					
					for (int sy = 0; sy < noise_yp; sy++)
					{
						double d10 = d1;
						double d11 = d2;
						double d12 = (d3 - d1) / (double)noise_xp;
						double d13 = (d4 - d2) / (double)noise_xp;
						
						for (int sz = 0; sz < noise_xp; sz++)
						{
							double d15 = d10;
							double d16 = (d11 - d10) / (double)noise_xp;
							
							for (int sx = 0; sx < noise_xp; sx++)
							{
								//int bx = (nx * noise_xp) + sx;
								//int by = (ny * noise_yp) + sy;
								//int bz = (nz * noise_xp) + sz;
								
								//Get block to place
								BlockId next_block;
								if (d15 > 0.0)
									next_block = BlockId_Stone;
								else
									next_block = BlockId_Air;
								
								//Place block
								*blockp++ = next_block;
								
								d15 += d16;
							}
							
							d10 += d12;
							d11 += d13;
							
							blockp += zinc;
						}
						
						d1 += d5;
						d2 += d6;
						d3 += d7;
						d4 += d8;
						
						blockp += yinc;
					}
				}
			}
		}
	}
	
	void ChunkGenerator::GenerateSurface(const ChunkPosition &pos, Block blocks[])
	{
		//Get noise
		double field_904_s[CHUNK_DIM * CHUNK_DIM] = {};
		field_909_n.Noise(field_904_s, pos.x * CHUNK_DIM, 109.0134, pos.z * CHUNK_DIM, CHUNK_DIM, 1, CHUNK_DIM, 0.03125, 1.0, 0.03125);
		
		double field_905_r[CHUNK_DIM * CHUNK_DIM] = {};
		double field_903_t[CHUNK_DIM * CHUNK_DIM] = {};
		field_909_n.Noise(field_905_r, pos.x * CHUNK_DIM, pos.z * CHUNK_DIM, 0.0, CHUNK_DIM, CHUNK_DIM, 1, 0.03125, 0.03125, 1.0);
		field_908_o.Noise(field_903_t, pos.x * CHUNK_DIM, pos.z * CHUNK_DIM, 0.0, CHUNK_DIM, CHUNK_DIM, 1, 0.03125 * 2.0, 0.03125 * 2.0, 0.03125 * 2.0);
		
		//Generate surface
		double *field_904_sp = field_904_s;
		double *field_905_rp = field_905_r;
		double *field_903_tp = field_903_t;
		
		for (int x = 0; x < CHUNK_DIM; x++)
		{
			for (int z = 0; z < CHUNK_DIM; z++)
			{
				Block *blockp = &blocks[(CHUNK_HEIGHT * (CHUNK_DIM * CHUNK_DIM)) + (z * CHUNK_DIM) + (x)];
				
				int j1 = -1;
				
				BlockId top_block = BlockId_Grass;
				BlockId filler_block = BlockId_Dirt;
				
				bool flag0 = (*field_905_rp++ + random.NextDouble() * 0.2) > 0.0;
				bool flag1 = (*field_904_sp++ + random.NextDouble() * 0.2) > 3.0;
				int i1 = (int)(*field_903_tp++ / 3.0 + 3.0 + random.NextDouble() * 0.25);
				
				for (int y = (CHUNK_HEIGHT - 1); y >= 0; y--)
				{
					//Shift down 1 block
					blockp -= CHUNK_DIM * CHUNK_DIM;
					
					//Get block and make sure it's a surface block
					BlockId block = (BlockId)(*blockp);
					if(block == BlockId_Air)
					{
						j1 = -1;
						continue;
					}
					if(block != BlockId_Stone)
						continue;
					
					//Get top and filler block
					if (j1 == -1)
					{
						if (i1 <= 0)
						{
							top_block = BlockId_Air;
							filler_block = BlockId_Stone;
						}
						else if(y >= water_level - 4 && y <= water_level + 1)
						{
							if (flag0)
							{
								top_block = BlockId_Sand;
								filler_block = BlockId_Sand;
							}
							else if (flag1)
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
						
						//if(k1 < water_level && top_block == Blocks.air)
						//	byte1 = Blocks.flowing_water;
						
						//Set top block
						j1 = i1;
						if (y >= water_level - 1)
							*blockp = top_block;
						else
							*blockp = filler_block;
					}
					else
					{
						//Set filler block
						if (j1 > 0)
						{
							*blockp = filler_block;
							j1--;
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
		
		//Initialize terrain noise
		field_912_k.SetOctaves(seed, 16);
		field_911_l.SetOctaves(seed, 16);
		field_910_m.SetOctaves(seed, 8);
		field_909_n.SetOctaves(seed, 4);
		field_908_o.SetOctaves(seed, 4);
		
		field_922_a.SetOctaves(seed, 10);
		field_921_b.SetOctaves(seed, 16);
		field_920_c.SetOctaves(seed, 8);
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
		
		//Get weather tables
		double temperature[CHUNK_DIM][CHUNK_DIM] = {};
		double humidity[CHUNK_DIM][CHUNK_DIM] = {};
		parent_chunk_manager.GetChunkBiome(pos, &(temperature[0][0]), &(humidity[0][0]));
		
		//Generate chunk step by step
		GenerateTerrain(pos, blocks, &(temperature[0][0]), &(humidity[0][0]));
		GenerateSurface(pos, blocks);
	}
}

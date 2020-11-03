#include "ChunkGenerator.h"
#include "ChunkManager.h"

namespace World
{
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
	
	//Terrain generate constants
	static const int noise_xp = 4;
	static const int noise_yp = 8;
	
	static const int noise_dim = (CHUNK_DIM + (noise_xp - 1)) / noise_xp;
	static const int noise_height = (CHUNK_HEIGHT + (noise_yp - 1)) / noise_yp;
	static const int water_level = 64;
	
	static const int noise_xd = noise_dim + 1;
	static const int noise_yd = noise_height + 1;
	static const int noise_zd = noise_dim + 1;
	
	static const double noise_field_factor = 684.412;
	
	//Internal interface
	void ChunkGenerator::InitializeNoiseField(double out[], int x, int y, int z, double temperature[], double humidity[])
	{
		//Generate noise tables
		double layer_2d_1[noise_xd * noise_zd] = {};
		double layer_2d_2[noise_xd * noise_zd] = {};
		field_922_a.Noise(layer_2d_1, x, z, noise_xd, noise_zd, 1.121, 1.121);
		field_921_b.Noise(layer_2d_2, x, z, noise_xd, noise_zd, 200.0, 200.0);
		
		double layer_3d_1[noise_xd * noise_zd * noise_yd] = {};
		double layer_3d_2[noise_xd * noise_zd * noise_yd] = {};
		double layer_3d_3[noise_xd * noise_zd * noise_yd] = {};
		field_910_m.Noise(layer_3d_1, x, y, z, noise_xd, noise_yd, noise_zd, noise_field_factor / 80.0, noise_field_factor / 160.0, noise_field_factor / 80.0);
		field_912_k.Noise(layer_3d_2, x, y, z, noise_xd, noise_yd, noise_zd, noise_field_factor,        noise_field_factor,         noise_field_factor);
		field_911_l.Noise(layer_3d_3, x, y, z, noise_xd, noise_yd, noise_zd, noise_field_factor,        noise_field_factor,         noise_field_factor);
		
		//Process noise
		int p_2d = 0;
		int p_3d = 0;
		
		int i2 = CHUNK_DIM / noise_xd;
		for (int nx = 0; nx < noise_xd; nx++)
		{
			int px = (nx * i2) + (i2 / 2);
			for (int nz = 0; nz < noise_zd; nz++)
			{
				int pz = (nz * i2) + (i2 / 2);
				
				//Handle weather at point
				double point_temp = temperature[px * CHUNK_DIM + pz];
				double point_humid =   humidity[px * CHUNK_DIM + pz] * point_temp;
				
				double humid_factor = 1.0 - point_humid;
				humid_factor *= humid_factor;
				humid_factor *= humid_factor;
				humid_factor = 1.0 - humid_factor;
				humid_factor *= 3.0;
				
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
				value_2d_2 = (value_2d_2 * (double)noise_yd) / (double)noise_height;
				double d7 = ((double)noise_yd / 2.0) + (value_2d_2 * 4.0);
				p_2d++;
				
				for (int ny = 0; ny < noise_yd; ny++)
				{
					//Handle 3D layers
					double d8 = 0.0;
					double d9 = (((double)ny - d7) * 12.0) / value_2d_1;
					if (d9 < 0.0)
						d9 *= 4.0;
					
					double value_3d_1 = (layer_3d_1[p_3d] / 512.0);
					double value_3d_2 = (layer_3d_2[p_3d] / 512.0);
					double value_3d_3 = (layer_3d_3[p_3d] / 10.0 + 1.0) / 2.0;
					
					if (value_3d_3 < 0.0)
						d8 = value_3d_1;
					else if (value_3d_3 > 1.0)
						d8 = value_3d_2;
					else
						d8 = value_3d_1 + (value_3d_2 - value_3d_1) * value_3d_3;
					d8 -= d9;
					
					if (ny > (noise_yd - 4))
					{
						double d13 = (double)(ny - (noise_yd - 4)) / 3.0;
						d8 = d8 * (1.0 - d13) + -10.0 * d13;
					}
					
					out[p_3d] = d8;
					p_3d++;
				}
			}
		}
	}
	
	//Chunk generator interface
	void ChunkGenerator::GenerateChunk(const ChunkPosition &pos, Block blocks[])
	{
		//Get weather tables
		double temperature[CHUNK_DIM][CHUNK_DIM] = {};
		double humidity[CHUNK_DIM][CHUNK_DIM] = {};
		parent_chunk_manager.GetChunkWeather(pos, &(temperature[0][0]), &(humidity[0][0]));
		
		//Get noise
		double noise[noise_xd][noise_zd][noise_yd] = {};
		InitializeNoiseField(&(noise[0][0][0]), pos.x * noise_dim, 0, pos.z * noise_dim, &(temperature[0][0]), &(humidity[0][0]));
		
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
					Block *blockp = &blocks[((ny * noise_xp) * (CHUNK_DIM * CHUNK_DIM)) + ((nz * noise_xp) * CHUNK_DIM) + (nx * noise_xp)];
					
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
}

#include "ChunkMesh.h"

namespace World
{
	//Block texture information
	struct BlockTexture
	{
		struct
		{
			int u;
			int v;
		} uv[6];
	};
	
	static const BlockTexture block_texture[BlockId_Num] = {
		//BlockFace_Front BlockFace_Right BlockFace_Back BlockFace_Left BlockFace_Top BlockFace_Bottom
		/* BlockId_Air */    {{{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0}}},
		/* BlockId_Stone */  {{{ 1, 0},{ 1, 0},{ 1, 0},{ 1, 0},{ 1, 0},{ 1, 0}}},
		/* BlockId_Grass */  {{{ 3, 0},{ 3, 0},{ 3, 0},{ 3, 0},{ 0, 0},{ 2, 0}}},
		/* BlockId_Dirt */   {{{ 2, 0},{ 2, 0},{ 2, 0},{ 2, 0},{ 2, 0},{ 2, 0}}},
		/* BlockId_Sand */   {{{ 2, 1},{ 2, 1},{ 2, 1},{ 2, 1},{ 2, 1},{ 2, 1}}},
		/* BlockId_Gravel */ {{{ 3, 1},{ 3, 1},{ 3, 1},{ 3, 1},{ 3, 1},{ 3, 1}}},
	};
	
	//Constructor and destructor
	ChunkMesh::ChunkMesh(Backend::Render::Render *_render) : render(_render)
	{
		return;
	}
	
	ChunkMesh::~ChunkMesh()
	{
		//Delete mesh
		delete mesh;
	}

	//Chunk mesh interface
	bool ChunkMesh::GenerateMesh(const ChunkMeshData &mesh_data)
	{
		//Convert chunk mesh data to generic mesh data
		std::vector<Backend::Render::Vertex> vert;
		for (auto &i : mesh_data.vert)
		{
			const BlockTexture &tex = block_texture[i.block];
			const float u = (float)(tex.uv[i.face].u + i.u) / 16.0f;
			const float v = (float)(tex.uv[i.face].v + i.v) / 16.0f;
			
			static const float side_lights[6] = {
				/* BlockFace_Front */  0.6f,
				/* BlockFace_Right */  0.8f,
				/* BlockFace_Back */   0.6f,
				/* BlockFace_Left */   0.8f,
				/* BlockFace_Top */    1.0f,
				/* BlockFace_Bottom */ 0.5f,
			};
			const float side_light = side_lights[i.face];
			
			vert.push_back({
				{i.x, i.y, i.z},
				{u, v},
				{0.0f, 0.0f, 0.0f},
				{side_light, side_light, side_light, 1.0f},
			});
		}
		
		//Create new mesh
		delete mesh;
		if ((mesh = render->NewMesh(vert, mesh_data.ind)) == nullptr)
			return error.Push("Failed to create mesh instance");
		else if (mesh->GetError())
			return error.Push(mesh->GetError());
		
		return false;
	}
}

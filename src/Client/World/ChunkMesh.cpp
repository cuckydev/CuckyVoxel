#include "ChunkMesh.h"

namespace World
{
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
			vert.push_back({
				{i.x, i.y, i.z},
				{0.0f, 0.0f},
				{0.0f, 0.0f, 0.0f},
				{i.r, i.g, i.b, 1.0f},
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

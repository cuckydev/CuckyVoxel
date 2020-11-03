#pragma once
#include "../Backend/Render.h"
#include <Common/World/Chunk.h>

#include <Common/Util/Error.h>

namespace World
{
	class ChunkMesh
	{
		private:
			//Error
			Error error;
			
			//Renderer and mesh
			Backend::Render::Render *render;
			Backend::Render::Mesh *mesh = nullptr;
			
		public:
			//Constructor and destructor
			ChunkMesh(Backend::Render::Render *_render);
			~ChunkMesh();
			
			//Chunk mesh interface
			bool GenerateMesh(const ChunkMeshData &mesh_data);
			Backend::Render::Mesh *GetMesh() { return mesh; }
			
			//Get error
			const Error &GetError() const { return error; }
	};
}

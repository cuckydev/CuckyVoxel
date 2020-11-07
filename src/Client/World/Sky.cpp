#include "Sky.h"
#include "../Main.h"
#include "../TextureData.h"

#include <cmath>
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>

namespace World
{
	//Sky constants
	static const float sky_rad = 256.0f;
	static const float sky_height = 16.0f;
	
	static const float cloud_dim = 12.0f;
	static const float cloud_depth = 4.0f;
	
	//Cloud mesh functions
	struct CloudMeshFace
	{
		float vertices[12];
		float light;
	};
	
	inline void MakeCloudFace(std::vector<Backend::Render::Vertex> &verts, std::vector<unsigned int> &inds, unsigned int &ind_i, const CloudMeshFace &face)
	{
		//Get vertices
		const float *vertex = face.vertices;
		for (int i = 0; i < 4; i++)
		{
			verts.push_back({
				{(float)(*vertex++) * cloud_dim, (float)(*vertex++) * cloud_depth, (float)(*vertex++) * cloud_dim},
				{0.0f, 0.0f},
				{0.0f, 0.0f, 0.0f},
				{face.light, face.light, face.light, 1.0f},
			});
		}
		
		//Push indices
		inds.insert(inds.end(), {ind_i, ind_i + 1, ind_i + 2, ind_i + 2, ind_i + 3, ind_i});
		ind_i += 4;
	}
	
	//Constructor and destructor
	Sky::Sky(Backend::Render::Render *_render) : render(_render)
	{
		//Load sky shader
		Backend::Render::ShaderFile sky_shader_file(executable_dir + "Data/Shader/Sky.shd");
		if (sky_shader_file.GetError())
		{
			error.Push(sky_shader_file.GetError());
			return;
		}
		
		sky_shader = render->NewShader(sky_shader_file);
		if (sky_shader == nullptr)
		{
			error.Push("Failed to create sky shader instance");
			return;
		}
		else if (sky_shader->GetError())
		{
			error.Push(sky_shader->GetError());
			return;
		}
		
		//Create sky meshes
		static const std::vector<unsigned int> mesh_inds = {
			0, 1, 2, 2, 3, 0
		};
		
		static const std::vector<Backend::Render::Vertex> ceil_verts = {
			{
				{sky_rad, sky_height, sky_rad},
				{0.0f, 0.0f},
				{0.0f, -1.0f, 0.0f},
				{1.0f, 1.0f, 1.0f, 1.0f},
			},
			{
				{-sky_rad, sky_height, sky_rad},
				{0.0f, 0.0f},
				{0.0f, -1.0f, 0.0f},
				{1.0f, 1.0f, 1.0f, 1.0f},
			},
			{
				{-sky_rad, sky_height, -sky_rad},
				{0.0f, 0.0f},
				{0.0f, -1.0f, 0.0f},
				{1.0f, 1.0f, 1.0f, 1.0f},
			},
			{
				{sky_rad, sky_height, -sky_rad},
				{0.0f, 0.0f},
				{0.0f, -1.0f, 0.0f},
				{1.0f, 1.0f, 1.0f, 1.0f},
			},
		};
		
		/*
		static const std::vector<Backend::Render::Vertex> void_verts = {
			{
				{sky_rad, -sky_height, -sky_rad},
				{0.0f, 0.0f},
				{0.0f, 1.0f, 0.0f},
				{1.0f, 1.0f, 1.0f, 1.0f},
			},
			{
				{-sky_rad, -sky_height, -sky_rad},
				{0.0f, 0.0f},
				{0.0f, 1.0f, 0.0f},
				{1.0f, 1.0f, 1.0f, 1.0f},
			},
			{
				{-sky_rad, -sky_height, sky_rad},
				{0.0f, 0.0f},
				{0.0f, 1.0f, 0.0f},
				{1.0f, 1.0f, 1.0f, 1.0f},
			},
			{
				{sky_rad, -sky_height, sky_rad},
				{0.0f, 0.0f},
				{0.0f, 1.0f, 0.0f},
				{1.0f, 1.0f, 1.0f, 1.0f},
			},
		};
		*/
		
		ceiling_mesh = render->NewMesh(ceil_verts, mesh_inds);
		if (ceiling_mesh == nullptr)
		{
			error.Push("Failed to create ceiling mesh instance");
			return;
		}
		else if (ceiling_mesh->GetError())
		{
			error.Push(ceiling_mesh->GetError());
			return;
		}
		
		/*
		void_mesh = render->NewMesh(void_verts, mesh_inds);
		if (void_mesh == nullptr)
		{
			error.Push("Failed to create void mesh instance");
			return;
		}
		else if (void_mesh->GetError())
		{
			error.Push(void_mesh->GetError());
			return;
		}
		*/
		
		//Load clouds texture
		TextureData clouds_texture_data(executable_dir + "Data/World/Clouds.png");
		if (clouds_texture_data.GetError())
		{
			error.Push(clouds_texture_data.GetError());
			return;
		}
		
		//Allocate cloud map
		cloud_width = clouds_texture_data.width;
		cloud_height = clouds_texture_data.height;
		if ((cloud_map = new uint8_t[cloud_width * cloud_height]) == nullptr)
		{
			error.Push("Failed to allocate cloud map");
			return;
		}
		
		//Read cloud map
		uint8_t *datap = clouds_texture_data.data + 3; //+3 so the alpha channel is read
		uint8_t *cloud_map_p = cloud_map;
		for (int y = 0; y < cloud_height; y++)
		{
			for (int x = 0; x < cloud_height; x++)
			{
				//Check if this pixel is set
				if (datap[(y * cloud_width + x) * 4] & 0x80)
				{
					//Check surrounding pixels
					bool xn, yn, xp, yp;
					
					if (x == 0)
						xn = datap[(y * cloud_width + (cloud_width - 1)) * 4] & 0x80; //X = cloud_width - 1
					else
						xn = datap[(y * cloud_width + (x - 1)) * 4] & 0x80; //X - 1
					
					if (y == 0)
						yn = datap[((cloud_height - 1) * cloud_width + x) * 4] & 0x80; //Y = cloud_height - 1
					else
						yn = datap[((y - 1) * cloud_width + x) * 4] & 0x80; //Y - 1
					
					if (x == (cloud_width - 1))
						xp = datap[(y * cloud_width + 0) * 4] & 0x80; //X = 0
					else
						xp = datap[(y * cloud_width + (x + 1)) * 4] & 0x80; //X + 1
					
					if (y == (cloud_height - 1))
						yp = datap[(0 * cloud_width + x) * 4] & 0x80; //Y = 0
					else
						yp = datap[((y + 1) * cloud_width + x) * 4] & 0x80; //Y + 1
					
					//Write map pixel
					*cloud_map_p++ = (xn ? 0x1 : 0x0) |
					                 (yn ? 0x2 : 0x0) |
					                 (xp ? 0x4 : 0x0) |
					                 (yp ? 0x8 : 0x0) |
					                 0x10;
				}
				else
				{
					//Write map pixel
					*cloud_map_p++ = 0;
				}
			}
		}
		
		//Cloud faces
		const CloudMeshFace cloud_front_face =  {{1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1}, 0.9f};
		const CloudMeshFace cloud_right_face =  {{1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0}, 0.95f};
		const CloudMeshFace cloud_back_face =   {{0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0}, 0.9f};
		const CloudMeshFace cloud_left_face =   {{0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, 0.95f};
		const CloudMeshFace cloud_top_face =    {{1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1}, 1.0f};
		const CloudMeshFace cloud_bottom_face = {{0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1}, 0.85f};
		
		//Generate cloud meshes
		for (int i = 0; i < 16; i++)
		{
			//Get surrounding pixels
			bool xn = i & 0x1;
			bool yn = i & 0x2;
			bool xp = i & 0x4;
			bool yp = i & 0x8;
			
			//Prepare to write mesh data
			std::vector<Backend::Render::Vertex> verts;
			std::vector<unsigned int> inds;
			unsigned int ind_i = 0;
			
			//Top and bottom faces
			MakeCloudFace(verts, inds, ind_i, cloud_top_face);
			MakeCloudFace(verts, inds, ind_i, cloud_bottom_face);
			
			//Side faces
			if (!xn)
				MakeCloudFace(verts, inds, ind_i, cloud_left_face);
			if (!yp)
				MakeCloudFace(verts, inds, ind_i, cloud_front_face);
			if (!xp)
				MakeCloudFace(verts, inds, ind_i, cloud_right_face);
			if (!yn)
				MakeCloudFace(verts, inds, ind_i, cloud_back_face);
			
			//Create mesh from mesh data
			if ((cloud_mesh[i] = render->NewMesh(verts, inds)) == nullptr)
			{
				error.Push("Failed to create cloud mesh instance");
				return;
			}
			else if (cloud_mesh[i]->GetError())
			{
				error.Push(cloud_mesh[i]->GetError());
				return;
			}
		}
	}
	
	Sky::~Sky()
	{
		//Delete render objects
		delete[] cloud_map;
		for (int i = 0; i < 16; i++)
			delete cloud_mesh[i];
		
		//delete void_mesh;
		delete ceiling_mesh;
		delete sky_shader;
	}
	
	//Sky interface
	float time_ticks = 0.0f;
	
	float GetLight(float celestial_angle)
	{
		float celestial_angle_factor = std::clamp(std::cos(celestial_angle * 6.283185f) * 2.0f + 0.5f, 0.0f, 1.0f);
		float val = (1.0f - celestial_angle_factor) * 11.0f;
		const float n2 = 1.0f - (15.0f - val) / 15.0f;
		return (1.0f - n2) / (n2 * 3.0f + 1.0f) * (1.0f - 0.05f) + 0.05f;
	}
	
	ColourSpace::RGB Sky::GetSkyColour()
	{
		float light = GetLight(sky_state.celestial_angle);
		return ColourSpace::HSVToRGB({
			0.622222f - sky_state.temperature * 0.05f,
			(1.0f - light * 0.5f) + sky_state.temperature * 0.1f,
			light * 1.1f - 0.1f,
		});
	}
	
	ColourSpace::RGB Sky::GetFogColour()
	{
		float celestial_angle_factor = std::clamp(std::cos(sky_state.celestial_angle * 6.283185f) * 2.0f + 0.5f, 0.0f, 1.0f);
		return {
			0.7529412f * celestial_angle_factor * 0.94f + 0.06f,
			0.8470588f * celestial_angle_factor * 0.94f + 0.06f,
			1.0f * celestial_angle_factor * 0.91f + 0.09f,
		};
	}
	
	ColourSpace::RGB Sky::GetAtmosphereColour()
	{
		ColourSpace::RGB sky_colour = GetSkyColour();
		ColourSpace::RGB fog_colour = GetFogColour();
		float light = GetLight(sky_state.celestial_angle);
		float blend_factor = 0.75f + (sky_state.blend_factor - 0.75f) * light;
		
		return {
			fog_colour.r + (sky_colour.r - fog_colour.r) * blend_factor,
			fog_colour.g + (sky_colour.g - fog_colour.g) * blend_factor,
			fog_colour.b + (sky_colour.b - fog_colour.b) * blend_factor,
		};
	}
	
	ColourSpace::RGB Sky::GetCloudColour()
	{
		float light = GetLight(sky_state.celestial_angle);
		return ColourSpace::HSVToRGB({
			0.7f,
			0.75f - light * 0.75f,
			0.2f + light * 0.8f,
		});
	}
	
	bool Sky::Render(glm::mat4 projection, glm::mat4 view, glm::dvec3 cam_pos)
	{
		//Get sky state
		time_ticks += 20.0f / 60.0f;
		if (time_ticks >= 24000.0f)
			time_ticks -= 24000.0f;
		
		float time_factor = (time_ticks / 24000.0f) - 0.25f;
		if (time_factor < 0.0f)
			time_factor += 1.0f;
		
		sky_state.celestial_angle = time_factor + ((1.0f - (std::cos(time_factor * 3.141592f) + 1.0f) / 2.0f) - time_factor) / 3.0f;
		sky_state.temperature = 0.75f;
		sky_state.blend_factor = 0.24f;
		
		float m22 = -projection[2][2];
		float m32 = -projection[3][2];    
		float far = (2.0f * m32) / (2.0f * m22 - 2.0f);
		float near = ((m22 - 1.0f) * far) / (m22 + 1.0f);
		
		//Get sky colours
		ColourSpace::RGB sky_colour = GetSkyColour();
		ColourSpace::RGB fog_colour = GetFogColour();
		ColourSpace::RGB atmosphere_colour = GetAtmosphereColour();
		ColourSpace::RGB cloud_colour = GetCloudColour();
		
		//Use sky shader
		sky_shader->Bind();
		sky_shader->SetUniform("u_projection", 1, &(projection[0][0]));
		sky_shader->SetUniform("u_view", 1, &(view[0][0]));
		
		glm::mat4 model(1.0f);
		sky_shader->SetUniform("u_model", 1, &(model[0][0]));
		
		sky_shader->SetUniform("u_fog_colour", atmosphere_colour.r, atmosphere_colour.g, atmosphere_colour.b, 1.0f);
		sky_shader->SetUniform("u_fog_start", 0.0f);
		sky_shader->SetUniform("u_fog_end", far * 0.8f);
		
		//Clear screen
		render->ClearColour(atmosphere_colour.r, atmosphere_colour.g, atmosphere_colour.b);
		render->ClearDepth(1.0f);
		
		//Draw ceiling plane
		sky_shader->SetUniform("u_col", sky_colour.r, sky_colour.g, sky_colour.b, 1.0f);
		ceiling_mesh->Draw();
		
		//Draw void plane
		//sky_shader->SetUniform("u_col", sky_colour.r * 0.2f + 0.04f, sky_colour.g * 0.2f + 0.04f, sky_colour.b * 0.6f + 0.1f, 1.0f);
		//void_mesh->Draw();
		
		//Clear depth so the rest of the scene draws on top of the sky
		render->ClearDepth(1.0f);
		
		//Get cloud positioning
		cloud_x += 0.02f;
		if (cloud_x >= cloud_width * cloud_dim)
			cloud_x -= cloud_width * cloud_dim;
		
		float cloud_y = (100.0f + cam_pos.y * 0.25f);
		float cloud_collerp = std::clamp(((float)(cloud_y - cam_pos.y) - 20.0f) / 50.0f, 0.0f, 1.0f);
		
		int cloud_cam_x = std::floor((cam_pos.x - cloud_x) / cloud_dim);
		int cloud_cam_z = std::floor(cam_pos.z / cloud_dim);
		int cloud_rad = std::ceil((far + cloud_dim * 0.707f) / cloud_dim);
		
		model = glm::translate(glm::mat4(1.0f), {
			-(cam_pos.x - cloud_x) + (float)(cloud_cam_x - cloud_rad) * cloud_dim,
			-cam_pos.y + cloud_y,
			-cam_pos.z + (float)(cloud_cam_z - cloud_rad) * cloud_dim
		});
		
		//Draw clouds
		sky_shader->SetUniform("u_col", cloud_colour.r, cloud_colour.g, cloud_colour.b, 0.65f);
		sky_shader->SetUniform("u_fog_colour",
			atmosphere_colour.r + (sky_colour.r - atmosphere_colour.r) * cloud_collerp,
			atmosphere_colour.g + (sky_colour.g - atmosphere_colour.g) * cloud_collerp,
			atmosphere_colour.b + (sky_colour.b - atmosphere_colour.b) * cloud_collerp,
			1.0f);
		
		for (int z = cloud_cam_z - cloud_rad; z <= cloud_cam_z + cloud_rad; z++)
		{
			for (int x = cloud_cam_x - cloud_rad; x <= cloud_cam_x + cloud_rad; x++)
			{
				if (glm::length(model[3]) <= (far + 0.707f * cloud_rad))
				{
					uint8_t map_val = cloud_map[((z + cloud_height) % cloud_height) * cloud_width + ((x + cloud_width) % cloud_width)];
					if (map_val & 0x10)
					{
						sky_shader->SetUniform("u_model", 1, &(model[0][0]));
						cloud_mesh[map_val & 0xF]->Draw();
					}
				}
				model[3][0] += cloud_dim;
			}
			model[3][0] -= cloud_dim * (cloud_rad + 1 + cloud_rad);
			model[3][2] += cloud_dim;
		}
		
		return false;
	}
}

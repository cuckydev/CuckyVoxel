#include "Sky.h"
#include "../Main.h"
#include "../TextureData.h"

#include <cmath>
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>

namespace World
{
	//Sky constants
	static const float sky_height = 16.0f;
	
	static const float sun_rad = 30.0f;
	static const float moon_rad = 20.0f;
	static const float body_dis = 100.0f;
	
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
		
		//Load generic texture shader
		Backend::Render::ShaderFile generic_texture_shader_file(executable_dir + "Data/Shader/GenericTexture.shd");
		if (generic_texture_shader_file.GetError())
		{
			error.Push(generic_texture_shader_file.GetError());
			return;
		}
		
		generic_texture_shader = render->NewShader(generic_texture_shader_file);
		if (generic_texture_shader == nullptr)
		{
			error.Push("Failed to create generic texture instance");
			return;
		}
		else if (generic_texture_shader->GetError())
		{
			error.Push(generic_texture_shader->GetError());
			return;
		}
		
		//Create sky meshes
		static const std::vector<Backend::Render::Vertex> ceil_verts = {
			{
				{1.0f, sky_height, 1.0f},
				{0.0f, 0.0f},
				{0.0f, -1.0f, 0.0f},
				{1.0f, 1.0f, 1.0f, 1.0f},
			},
			{
				{-1.0f, sky_height, 1.0f},
				{0.0f, 0.0f},
				{0.0f, -1.0f, 0.0f},
				{1.0f, 1.0f, 1.0f, 1.0f},
			},
			{
				{-1.0f, sky_height, -1.0f},
				{0.0f, 0.0f},
				{0.0f, -1.0f, 0.0f},
				{1.0f, 1.0f, 1.0f, 1.0f},
			},
			{
				{1.0f, sky_height, -1.0f},
				{0.0f, 0.0f},
				{0.0f, -1.0f, 0.0f},
				{1.0f, 1.0f, 1.0f, 1.0f},
			},
		};
		
		static const std::vector<unsigned int> ceil_inds = {
			0, 1, 2, 2, 3, 0
		};
		
		ceiling_mesh = render->NewMesh(ceil_verts, ceil_inds);
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
		
		static const std::vector<Backend::Render::Vertex> body_verts = {
			{
				{1.0f, body_dis, 1.0f},
				{0.0f, 0.0f},
				{0.0f, -1.0f, 0.0f},
				{1.0f, 1.0f, 1.0f, 1.0f},
			},
			{
				{-1.0f, body_dis, 1.0f},
				{1.0f, 0.0f},
				{0.0f, -1.0f, 0.0f},
				{1.0f, 1.0f, 1.0f, 1.0f},
			},
			{
				{-1.0f, body_dis, -1.0f},
				{1.0f, 1.0f},
				{0.0f, -1.0f, 0.0f},
				{1.0f, 1.0f, 1.0f, 1.0f},
			},
			{
				{1.0f, body_dis, -1.0f},
				{0.0f, 1.0f},
				{0.0f, -1.0f, 0.0f},
				{1.0f, 1.0f, 1.0f, 1.0f},
			},
		};
		
		static const std::vector<unsigned int> body_inds = {
			0, 1, 2, 2, 3, 0
		};
		
		body_mesh = render->NewMesh(body_verts, body_inds);
		if (body_mesh == nullptr)
		{
			error.Push("Failed to create body mesh instance");
			return;
		}
		else if (body_mesh->GetError())
		{
			error.Push(body_mesh->GetError());
			return;
		}
		
		//Load sun texture
		TextureData sun_texture_data(executable_dir + "Data/World/Sun.png");
		if (sun_texture_data.GetError())
		{
			error.Push(sun_texture_data.GetError());
			return;
		}
		
		sun_texture = render->NewTexture(sun_texture_data.data, sun_texture_data.width, sun_texture_data.height);
		if (sun_texture == nullptr)
		{
			error.Push("Failed to create sun texture instance");
			return;
		}
		else if (sun_texture->GetError())
		{
			error.Push(sun_texture->GetError());
			return;
		}
		
		//Load moon texture
		TextureData moon_texture_data(executable_dir + "Data/World/Moon.png");
		if (moon_texture_data.GetError())
		{
			error.Push(moon_texture_data.GetError());
			return;
		}
		
		moon_texture = render->NewTexture(moon_texture_data.data, moon_texture_data.width, moon_texture_data.height);
		if (moon_texture == nullptr)
		{
			error.Push("Failed to create moon texture instance");
			return;
		}
		else if (moon_texture->GetError())
		{
			error.Push(moon_texture->GetError());
			return;
		}
		
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
			for (int x = 0; x < cloud_width; x++)
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
		static const CloudMeshFace cloud_front_face =  {{1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1}, 0.9f};
		static const CloudMeshFace cloud_right_face =  {{1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0}, 0.95f};
		static const CloudMeshFace cloud_back_face =   {{0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0}, 0.9f};
		static const CloudMeshFace cloud_left_face =   {{0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, 0.95f};
		static const CloudMeshFace cloud_top_face =    {{1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1}, 1.0f};
		static const CloudMeshFace cloud_bottom_face = {{0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1}, 0.85f};
		
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
		
		delete moon_texture;
		delete sun_texture;
		
		delete body_mesh;
		delete ceiling_mesh;
		
		delete generic_texture_shader;
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
		//Get base colours
		ColourSpace::RGB sky_colour = GetSkyColour();
		ColourSpace::RGB fog_colour = GetFogColour();
		float light = GetLight(sky_state.celestial_angle);
		float blend_factor = 0.75f + (sky_state.blend_factor - 0.75f) * light;
		
		/*
		//Blend for sunrise and sunset
		float glow_fac = 0.0f;
		if (sky_state.celestial_angle < 0.5f)
			glow_fac = std::clamp(1.0f - std::abs(sky_state.celestial_angle - 0.25f) / 0.05f, 0.0f, 1.0f);
		else
			glow_fac = std::clamp(1.0f - std::abs(sky_state.celestial_angle - 0.75f) / 0.05f, 0.0f, 1.0f);
		
		static const ColourSpace::RGB sunset_col = {
			1.5f,
			0.35f,
			0.15f,
		};
		
		blend_factor *= 0.1f + glow_fac * 0.9f;
		fog_colour.r = fog_colour.r + (sunset_col.r - fog_colour.r) * glow_fac;
		fog_colour.g = fog_colour.g + (sunset_col.g - fog_colour.g) * glow_fac;
		fog_colour.b = fog_colour.b + (sunset_col.b - fog_colour.b) * glow_fac;
		*/
		
		//Return final blend colour
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
		//Model matrix
		glm::mat4 model;
		
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
		
		sky_shader->SetUniform("u_fog_colour", atmosphere_colour.r, atmosphere_colour.g, atmosphere_colour.b, 1.0f);
		sky_shader->SetUniform("u_fog_start", 0.0f);
		sky_shader->SetUniform("u_fog_end", far * 0.8f);
		
		//Clear screen
		render->ClearColour(atmosphere_colour.r, atmosphere_colour.g, atmosphere_colour.b);
		render->ClearDepth(1.0f);
		
		//Draw ceiling plane
		model = glm::scale(glm::mat4(1.0f), {far, 1.0f, far});
		sky_shader->SetUniform("u_model", 1, &(model[0][0]));
		
		sky_shader->SetUniform("u_col", sky_colour.r, sky_colour.g, sky_colour.b, 1.0f);
		ceiling_mesh->Draw();
		
		//Clear depth before rendering celestial bodies
		render->ClearDepth(1.0f);
		
		generic_texture_shader->Bind();
		generic_texture_shader->SetUniform("u_projection", 1, &(projection[0][0]));
		generic_texture_shader->SetUniform("u_view", 1, &(view[0][0]));
		
		//Sun
		model = glm::scale(glm::rotate(glm::mat4(1.0f), sky_state.celestial_angle * 6.283185f, {1.0f, 0.0f, 0.0f}), {sun_rad, 1.0f, sun_rad});
		generic_texture_shader->SetUniform("u_model", 1, &(model[0][0]));
		
		sun_texture->Bind();
		body_mesh->Draw();
		
		//Moon
		model = glm::scale(glm::rotate(glm::mat4(1.0f), (sky_state.celestial_angle + 0.5f) * 6.283185f, {1.0f, 0.0f, 0.0f}), {moon_rad, 1.0f, moon_rad});
		generic_texture_shader->SetUniform("u_model", 1, &(model[0][0]));
		
		moon_texture->Bind();
		body_mesh->Draw();
		
		//Clear depth before rendering clouds
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
		
		//Draw clouds
		sky_shader->Bind();
		sky_shader->SetUniform("u_col", cloud_colour.r, cloud_colour.g, cloud_colour.b, 0.65f);
		sky_shader->SetUniform("u_fog_start", std::clamp((float)(cloud_y - cam_pos.y), 0.0f, far * 0.5f));
		sky_shader->SetUniform("u_fog_colour",
			atmosphere_colour.r + (sky_colour.r - atmosphere_colour.r) * cloud_collerp,
			atmosphere_colour.g + (sky_colour.g - atmosphere_colour.g) * cloud_collerp,
			atmosphere_colour.b + (sky_colour.b - atmosphere_colour.b) * cloud_collerp,
			0.0f);
		
		static const int pos[5][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 0}};
		
		for (int x = 0; x <= cloud_rad * 3 / 2; x++)
		{
			if (x == 0)
			{
				int mx = (cloud_width + ((cloud_cam_x + cloud_width) % cloud_width)) % cloud_width;
				int mz = (cloud_height + ((cloud_cam_z + cloud_height) % cloud_height)) % cloud_height;
				uint8_t map_val = cloud_map[mz * cloud_width + mx];
				if (map_val & 0x10)
				{
					model = glm::translate(glm::mat4(1.0f), {
						-cam_pos.x + (float)cloud_cam_x * cloud_dim + cloud_x,
						-cam_pos.y + cloud_y,
						-cam_pos.z + (float)cloud_cam_z * cloud_dim
					});
					if (glm::length(model[3]) <= (far + 0.707f * cloud_rad))
					{
						sky_shader->SetUniform("u_model", 1, &(model[0][0]));
						cloud_mesh[map_val & 0xF]->Draw();
					}
				}
			}
			else
			{
				for (int i = 0; i < 4; i++)
				{
					for (int v = 0; v < x; v++)
					{
						int wx = cloud_cam_x + pos[i][0] * (x - v) + pos[i + 1][0] * v;
						int wz = cloud_cam_z + pos[i][1] * (x - v) + pos[i + 1][1] * v;
						int mx = (cloud_width + ((wx + cloud_width) % cloud_width)) % cloud_width;
						int mz = (cloud_height + ((wz + cloud_height) % cloud_height)) % cloud_height;
						uint8_t map_val = cloud_map[mz * cloud_width + mx];
						if (map_val & 0x10)
						{
							model = glm::translate(glm::mat4(1.0f), {
								-cam_pos.x + (float)wx * cloud_dim + cloud_x,
								-cam_pos.y + cloud_y,
								-cam_pos.z + (float)wz * cloud_dim
							});
							if (glm::length(model[3]) <= (far + 0.707f * cloud_rad))
							{
								sky_shader->SetUniform("u_model", 1, &(model[0][0]));
								cloud_mesh[map_val & 0xF]->Draw();
							}
						}
					}
				}
			}
		}
		
		return false;
	}
}

#pragma once
#include "RawModel.h"
#include "ModelTexture.h"
#include "Loader.h"
#include "TerrainTexture.h"
#include "TerrainTexturePack.h"
#include "stb_image.hpp"


class Terrain
{
	private:
		const static int SIZE = 800;
		const static int MAX_HEIGHT = 40;
		const static int MAX_PIXEL_COLOR = 256 * 256 * 256;
	

		int x;
		int z;
		RawModel model;
		TerrainTexture blendMap;
		TerrainTexturePack texturePack;

		float get_height(int x, int z, const stb::image& image)
		{
			if (x < 0 || x > image.height() || z < 0 || z > image.height()) {
				return 0;
			}

			uint32_t val = image.get_rgb(x, z);

			double height = val;
			height /= (MAX_PIXEL_COLOR / 2);
			height -= 1.0;
			height *= MAX_HEIGHT;
			return height;
		}

		glm::vec3 calculateNormal(int x, int z, const stb::image& image)
		{
			float height_l = get_height(x - 1, z, image);
			float height_r = get_height(x + 1, z, image);
			float height_d = get_height(x, z - 1, image);
			float height_u = get_height(x, z + 1, image);

			glm::vec3 normal{ height_l - height_r, 2.0f, height_d - height_u };
			normal = glm::normalize(normal);

			return normal;
		}

	public:
		Terrain(int gridX, int gridZ, Loader loader, TerrainTexturePack texturePack, TerrainTexture blendMap, const std::string& heightmap)
		{
			this->texturePack = texturePack;
			this->blendMap = blendMap;
			this->x = gridX * SIZE;
			this->z = gridZ * SIZE;
			this->model = generateTerrain(loader, heightmap);
		}


		RawModel generateTerrain(Loader& loader, const std::string& heightmap)
		{
			stb::image image{ "src/Resources/Textures/HeightMaps/" + heightmap + ".png", 4 };
			int VERTEX_COUNT = image.height();
			int count = VERTEX_COUNT * VERTEX_COUNT;
			std::vector<glm::vec3> vertices(count), normals(count);
			std::vector<glm::vec2> textureCoords(count);
			std::vector<int> indices(6 * (VERTEX_COUNT - 1) * VERTEX_COUNT);
			int vertexPointer = 0;
			for (int i = 0; i < VERTEX_COUNT; i++)
			{
				for (int j = 0; j < VERTEX_COUNT; j++)
				{
					vertices[vertexPointer] = glm::vec3( (float)j / ((float)VERTEX_COUNT - 1) * SIZE, get_height(j, i, image), (float)i / ((float)VERTEX_COUNT - 1) * SIZE);
					normals[vertexPointer] = calculateNormal(j, i, image);
					textureCoords[vertexPointer] = glm::vec2( (float)j / ((float)VERTEX_COUNT - 1), (float)i / ((float)VERTEX_COUNT - 1));
					vertexPointer++;
				}
			}
			int pointer = 0;
			for (int gz = 0; gz < VERTEX_COUNT - 1; gz++)
			{
				for (int gx = 0; gx < VERTEX_COUNT - 1; gx++)
				{
					int topLeft = (gz * VERTEX_COUNT) + gx;
					int topRight = topLeft + 1;
					int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
					int bottomRight = bottomLeft + 1;
					indices[pointer++] = topLeft;
					indices[pointer++] = bottomLeft;
					indices[pointer++] = topRight;
					indices[pointer++] = topRight;
					indices[pointer++] = bottomLeft;
					indices[pointer++] = bottomRight;
				}
			}
			return loader.loadToVAO(vertices, textureCoords, normals, indices);
		}

		int getX()
		{
			return x;
		}

		int getZ()
		{
			return z;
		}

		RawModel getModel()
		{
			return model;
		}

		TerrainTexture getBlendMap()
		{
			return blendMap;
		}

		TerrainTexturePack getTexturePack()
		{
			return texturePack;
		}
};
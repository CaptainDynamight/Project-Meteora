#pragma once
#include "Loader.h"
#include "RawModel.h"
#include <vector>
#include <istream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <time.h>

struct Vertex
{
	glm::fvec3 position;
	glm::fvec3 color;
	glm::fvec2 texcoord;
	glm::fvec3 normal;
};


class OBJLoader
{
public:
	RawModel loadOBJ(const std::string& fileName, Loader& loader)
	{
		std::string path = "src/Resources/Models/" + fileName;
		FILE* file;
		if (fopen_s(&file, path.c_str(), "r") != 0)
		{
			std::cout << "Failed to open : " + fileName << std::endl;
		}
		std::vector<glm::vec2> textures, tempTextures;
		std::vector<glm::vec3> vertices, normals, tempNormals;
		std::vector<int> indices;

		char* type, * token, * stop = 0;
		double x, y, z;
		char line[256];
		while (fgets(line, 256, file) != NULL)
		{
			token = NULL;
			type = strtok_s(line, " ", &token);
			if (type[0] == 'v' && type[1] == NULL)
			{
				x = strtod(token, &stop);
				token = stop + 1;
				y = strtod(token, &stop);
				token = stop + 1;
				z = strtod(token, &stop);
				vertices.push_back(glm::vec3(x, y, z));
			}
			else if (type[0] == 'v' && type[1] == 't')
			{
				x = strtod(token, &stop);
				token = stop + 1;
				y = 1 - strtod(token, &stop);
				tempTextures.push_back(glm::vec2(x, y));
			}
			else if (type[0] == 'v' && type[1] == 'n')
			{
				x = strtod(token, &stop);
				token = stop + 1;
				y = strtod(token, &stop);
				token = stop + 1;
				z = strtod(token, &stop);
				tempNormals.push_back(glm::vec3(x, y, z));
			}
			else if (type[0] == 'f')
			{
				if (indices.size() == 0)
				{
					textures.resize(vertices.size());
					normals.resize(vertices.size());
				}
				ProcessVertices(token, indices, tempTextures, textures, tempNormals, normals);
			}
		}
		fclose(file);

		return loader.loadToVAO(vertices, textures, normals, indices);
	}


	void ProcessVertices(char* vertexData, std::vector<int>& indices, std::vector<glm::vec2>& tempTextures,
		std::vector<glm::vec2>& textures, std::vector<glm::vec3>& tempNormals, std::vector<glm::vec3>& normals)
	{
		char* stop;
		int vertexPointer;
		for (unsigned int i = 0; i < 3; i++)
		{
			vertexPointer = strtol(vertexData, &stop, 10) - 1;
			indices.push_back(vertexPointer);
			vertexData = stop + 1;
			textures[vertexPointer] = tempTextures[strtol(vertexData, &stop, 10) - 1];
			vertexData = stop + 1;
			normals[vertexPointer] = tempNormals[strtol(vertexData, &stop, 10) - 1];
			vertexData = stop + 1;
		}
	}
};

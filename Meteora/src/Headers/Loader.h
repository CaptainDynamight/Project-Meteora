#pragma once

#include "RawModel.h"
#include <GL/glew.h>
#include <iostream>
#include <iterator>
#include <vector>
#include <list>
#include "glm\glm.hpp"
#include "stb_image.h"


class Loader
{
private:
	std::list<unsigned int> vaos;
	std::list<unsigned int> vbos;
	std::list<unsigned int> textures;

public:

	unsigned int loadTexture(const std::string& fileName)
	{
		std::string path = "src/Resources/Textures/" + fileName;
		unsigned int textureID;
		int width, height, nrChannels;
		unsigned char* imageData = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);
		if (imageData == NULL)
			std::cerr << "ERROR: texture loading failed for " << fileName << std::endl;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		stbi_set_flip_vertically_on_load(true);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
		textures.push_back(textureID);
		stbi_image_free(imageData);
		return textureID;
	}


	RawModel loadToVAO(std::vector<glm::vec3> vertices, std::vector<glm::vec2> textures, std::vector<glm::vec3> normals, std::vector<int> indices)
	{
		// create a new VAO
		GLuint vaoID = createVAO();
		int indicesSize = indices.size();
		BindIndicesBuffer(indices.data(), indicesSize);
		// Store the data in attribute lists
		storeDataInAttributeList(0, 3, &vertices[0], vertices.size() * sizeof(glm::vec3));
		storeDataInAttributeList(1, 2, &textures[0], textures.size() * sizeof(glm::vec2));
		storeDataInAttributeList(2, 3, &normals[0], normals.size() * sizeof(glm::vec3));
		unbindVAO();
		return RawModel(vaoID, indicesSize);
	}

	void cleanUp()
	{
		for (unsigned int vao : vaos)
		{
			glDeleteVertexArrays(1, &vao);
		}
		for (unsigned int vbo : vbos)
		{
			glDeleteBuffers(1, &vbo);
		}
		for (unsigned int texture : textures)
		{
			glDeleteTextures(1, &texture);
		}
	}

	unsigned int createVAO()
	{
		unsigned int vaoID;
		glGenVertexArrays(1, &vaoID);
		vaos.push_back(vaoID);
		glBindVertexArray(vaoID);
		return vaoID;
	}

	void unbindVAO()
	{
		glBindVertexArray(0);
	}

	void storeDataInAttributeList(GLuint attribNumber, int attribSize, void* data, int dataSize)
	{
		unsigned int vboID;
		glGenBuffers(1, &vboID);
		vbos.push_back(vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
		glVertexAttribPointer(attribNumber, attribSize, GL_FLOAT, GL_FALSE, 0, nullptr);
	}


	void BindIndicesBuffer(int* indices, int& count)
	{
		unsigned int vboID;
		glGenBuffers(1, &vboID);
		vbos.push_back(vboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * count, indices, GL_STATIC_DRAW);
	}
};
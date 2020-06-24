#pragma once
#include "ShaderProgram.h"

class TerrainShader : public ShaderProgram
{
private:
	const std::string& VERTEX_FILE = "src/Shaders/terrainVertexShader.txt";
	const std::string& FRAGMENT_FILE = "src/shaders/terrainFragmentShader.txt";
	std::string VERTEX_SOURCE = parseShader(VERTEX_FILE);
	std::string FRAGMENT_SOURCE = parseShader(FRAGMENT_FILE);

	unsigned int programID;
	unsigned int vertexShaderID;
	unsigned int fragmentShaderID;
	unsigned int location_transformationMatrix;
	unsigned int location_projectionMatrix;
	unsigned int location_viewMatrix;
	unsigned int location_lightPosition;
	unsigned int location_lightColor;
	unsigned int location_shineDamper;
	unsigned int location_reflectivity;
	unsigned int location_skyColor;
	unsigned int location_backgroundTexture;
	unsigned int location_rTexture;
	unsigned int location_gTexture;
	unsigned int location_bTexture;
	unsigned int location_blendMap;

public:

	TerrainShader()
	{
		programID = glCreateProgram();
		vertexShaderID = loadShader(VERTEX_SOURCE, GL_VERTEX_SHADER);
		fragmentShaderID = loadShader(FRAGMENT_SOURCE, GL_FRAGMENT_SHADER);
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		bindAttributes();
		glLinkProgram(programID);
		glValidateProgram(programID);
		getAllUniformLocations();
	}

	unsigned int getProgramID()
	{
		return programID;
	}

	void start()
	{
		glUseProgram(programID);
	}

	void stop()
	{
		glUseProgram(0);
	}

	void cleanUp()
	{
		stop();
		glDetachShader(programID, vertexShaderID);
		glDetachShader(programID, fragmentShaderID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		glDeleteProgram(programID);
	}

	void getAllUniformLocations()
	{
		location_transformationMatrix = getUniformLocation("transformationMatrix");
		location_projectionMatrix = getUniformLocation("projectionMatrix");
		location_viewMatrix = getUniformLocation("viewMatrix");
		location_lightPosition = getUniformLocation("lightPosition");
		location_lightColor = getUniformLocation("lightColor");
		location_shineDamper = getUniformLocation("shineDamper");
		location_reflectivity = getUniformLocation("reflectivity");
		location_skyColor = getUniformLocation("skyColor");
		location_backgroundTexture = getUniformLocation("backgroundTexture");
		location_rTexture = getUniformLocation("rTexture");
		location_gTexture = getUniformLocation("gTexture");
		location_bTexture = getUniformLocation("bTexture");
		location_blendMap = getUniformLocation("blendMap");
	}

	void connectTextureUnits()
	{
		loadInt(location_backgroundTexture, 0);
		loadInt(location_rTexture, 1);
		loadInt(location_gTexture, 2);
		loadInt(location_bTexture, 3);
		loadInt(location_blendMap, 4);
	}

	void loadSkyColor(float r, float g, float b)
	{
		loadVector(location_skyColor, glm::vec3(r, g, b));
	}

	void loadShineVariables(float shineDamper, float reflectivity)
	{
		loadFloat(location_shineDamper, shineDamper);
		loadFloat(location_reflectivity, reflectivity);
	}

	void loadLight(Light light)
	{
		loadVector(location_lightPosition, light.getPosition());
		loadVector(location_lightColor, light.getColor());
	}

	void loadProjectionMatrix(glm::mat4& matrix)
	{
		loadMatrix(location_projectionMatrix, matrix);
	}

	void loadTransformationMatrix(glm::mat4& matrix)
	{
		loadMatrix(location_transformationMatrix, matrix);
	}

	void loadViewMatrix(Camera camera)
	{
		glm::mat4 viewMatrix = camera.createViewMatrix();
		loadMatrix(location_viewMatrix, viewMatrix);
	}

	unsigned int getUniformLocation(const std::string& uniformName)
	{
		return glGetUniformLocation(programID, uniformName.c_str());
	}
};
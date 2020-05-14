#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL\glew.h>
#include "glm\glm.hpp"
#include "Camera.h"
#include "Maths.h"
#include "Light.h"

class ShaderProgram
{
	private:
		const std::string& VERTEX_FILE = "src/Shaders/vertexShader.txt";
		const std::string& FRAGMENT_FILE = "src/shaders/fragmentShader.txt";
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
		unsigned int location_useFakeLighting;
		unsigned int location_skyColor;

	public:
		static unsigned int loadShader(const std::string& source, unsigned int type)
		{
			unsigned int shaderID = glCreateShader(type);
			const char* src = source.c_str();

			glShaderSource(shaderID, 1, &src, nullptr);
			glCompileShader(shaderID);

			int result;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

			if (result == GL_FALSE)
			{
				int length;
				glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
				char* message = (char*)_malloca(length * sizeof(char));
				glGetShaderInfoLog(shaderID, length, &length, message);
				std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? " Vertex " : " Fragment ") << "shader" << std::endl;
				std::cout << message << std::endl;
				glDeleteShader(shaderID);
				return 0;
			}
			return shaderID;
		}


		static std::string parseShader(const std::string& filepath)
		{
			std::stringstream ss;
			std::ifstream stream(filepath);
			std::string line;
			while (getline(stream, line))
			{
				ss << line << "\n";
			}
			return ss.str();
		}

		ShaderProgram()
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
			location_useFakeLighting = getUniformLocation("useFakeLighting");
			location_skyColor = getUniformLocation("skyColor");
		}

		void loadSkyColor(float r, float g, float b)
		{
			loadVector(location_skyColor, glm::vec3(r, g, b));
		}

		void loadFakeLightingVariable(bool useFakeLighting)
		{
			loadBoolean(location_useFakeLighting, useFakeLighting);
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
			glm::mat4 viewMatrix = Camera::createViewMatrix(camera);
			loadMatrix(location_viewMatrix, viewMatrix);
		}

		void bindAttributes()
		{
			bindAttributes(0, "position");
			bindAttributes(1, "textureCoords");
			bindAttributes(2, "normal");
		}

		void bindAttributes(unsigned int attribute, const std::string& variableName)
		{
			glBindAttribLocation(programID, attribute, variableName.c_str());
		}

		unsigned int getUniformLocation(const std::string& uniformName)
		{
			return glGetUniformLocation(programID, uniformName.c_str());
		}

		void loadFloat(unsigned int location, float value)
		{
			glUniform1f(location, value);
		}

		void loadInt(unsigned int location, int value)
		{
			glUniform1i(location, value);
		}

		void loadVector(unsigned int location, glm::vec3 value)
		{
			glUniform3f(location, value.x, value.y, value.z);
		}

		void loadBoolean(unsigned int location, bool value)
		{
			glUniform1f(location, value ? 1.0f : 0.0f);
		}

		void loadMatrix(unsigned int location, glm::mat4& matrix)
		{
			glUniformMatrix4fv(location, 1, false, &matrix[0][0]);
		}
};
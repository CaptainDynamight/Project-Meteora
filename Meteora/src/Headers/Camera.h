#pragma once
#include "glm\glm.hpp"
#include <GLFW\glfw3.h>
#include <glm\gtx\transform.hpp>
#include <GL\glew.h>

glm::vec3 position = glm::vec3(0, 0, 0);

class Camera
{
	private:
		float pitch, yaw, roll;
		bool m_wireframe = false;

	public:
		Camera() = default;

		glm::vec3 getPosition()
		{
			return position;
		}

		float getPitch()
		{
			return pitch;
		}

		float getYaw()
		{
			return yaw;
		}

		float getRoll()
		{
			return roll;
		}

		void move()
		{
			if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
			{
				position.z -= 1.0f;
			}
			if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
			{
				position.z += 1.0f;
			}
			if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT) == GLFW_PRESS)
			{
				position.x -= 1.0f;
			}
			if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT) == GLFW_PRESS)
			{
				position.x += 1.0f;
			}
			if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP) == GLFW_PRESS)
			{
				position.y += 1.0f;
			}
			if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN) == GLFW_PRESS)
			{
				position.y -= 1.0f;
			}
			if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_F6) == GLFW_PRESS)
			{
				if (m_wireframe)
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				else
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				m_wireframe = !m_wireframe;
			}
		}

		static glm::mat4 createViewMatrix(Camera camera)
		{
			glm::mat4 viewMatrix = glm::lookAt(position, position + glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
			return viewMatrix;
		}
};
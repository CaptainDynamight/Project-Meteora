#pragma once
#include "Entity.h"
#include "DisplayManager.h"
#include "GameManager.h"
#include <GLFW\glfw3.h>

class Player : public Entity
{
	public : 
		Player() = default;

		Player(TexturedModel model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : Entity(model, position, rotation, scale) 
		{
		}

		void move()
		{
			checkInputs();
			increaseRotation(glm::vec3(0, currentTurnSpeed * DisplayManager::getFrameTimeSeconds(), 0));
			double distance = currentRunSpeed * DisplayManager::getFrameTimeSeconds();
			double dx = (double) distance * sin(Entity::getRotation().y);
			double dz = (double) distance * cos(Entity::getRotation().y);
			increasePosition(glm::vec3(dx, 0, dz));
			currentJumpSpeed += GRAVITY * DisplayManager::getFrameTimeSeconds();
			increasePosition(glm::vec3(0, currentJumpSpeed * DisplayManager::getFrameTimeSeconds(), 0));
			if (getPosition().y < TERRAIN_HEIGHT)
			{
				currentJumpSpeed = 0;
				setPosition(glm::vec3(getPosition().x, 0, getPosition().z));
				isInAir = false;
			}
		}

	private :
		const double RUN_SPEED = 20;
		const double TURN_SPEED = 2;
		const double GRAVITY = -50;
		const double JUMP_POWER = 30;
		const double TERRAIN_HEIGHT = 0;
		double currentRunSpeed = 0;
		double currentTurnSpeed = 0;
		double currentJumpSpeed = 0;
		bool isInAir = false;

		void jump()
		{
			if (!isInAir)
			{
				this->currentJumpSpeed = JUMP_POWER;
				isInAir = true;
			}
		}

		void checkInputs()
		{
			if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
			{
				this->currentRunSpeed = RUN_SPEED;
			}
			else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
			{
				this->currentRunSpeed = -RUN_SPEED;
			}
			else
			{
				this->currentRunSpeed = 0;
			}

			if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
			{
				this->currentTurnSpeed = TURN_SPEED;
			}
			else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
			{
				this->currentTurnSpeed = -TURN_SPEED;
			}
			else
			{
				this->currentTurnSpeed = 0;
			}
			if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS)
			{
				jump();
			}
		}
};
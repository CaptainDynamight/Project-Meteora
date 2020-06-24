#pragma once

#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include "DisplayManager.h"
#include "Loader.h"
#include "RawModel.h"
#include "ShaderProgram.h"
#include "ModelTexture.h"
#include "TexturedModel.h"
#include "Entity.h"
#include "Camera.h"
#include "OBJLoader.h"
#include "Light.h"
#include "MasterRenderer.h"
#include "Terrain.h"
#include "Terrain.h"
#include "TerrainTexture.h"
#include "TerrainTexturePack.h"
#include "Player.h"


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//Camera camera;
Light light(glm::vec3(0, 50, 0), glm::vec3(1, 1, 1));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool m_wireframe = 1;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


// glfw: whenever the window size changed(by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
extern void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
extern void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
extern void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
	light.setPosition(glm::vec3(rand() % 500 - 250, rand() % 500 - 250, rand() % 500 - 250));
	light.setColor(glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX));
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, DisplayManager::getFrameTimeSeconds());
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, DisplayManager::getFrameTimeSeconds());
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, DisplayManager::getFrameTimeSeconds());
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, DisplayManager::getFrameTimeSeconds());

	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_F6) == GLFW_PRESS)
	{
		if (m_wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		m_wireframe = !m_wireframe;
	}
}



class GameManager
{
	public:
		GameManager()
		{
			// Initialized GLFW
			if (glfwInit())
			{
				std::cout << "GLFW initialized successfully" << std::endl;

				// Create the display manager (pointers must be deleted)
				m_displayManager = new DisplayManager(SCR_WIDTH, SCR_HEIGHT, "Meteora");
				glfwSetFramebufferSizeCallback(glfwGetCurrentContext(), framebuffer_size_callback);
				glfwSetCursorPosCallback(glfwGetCurrentContext(), mouse_callback);
				glfwSetScrollCallback(glfwGetCurrentContext(), scroll_callback);
				// Initialize glew using experimental(new)
				glewExperimental = true;
				GLenum status = glewInit();
				if (status == GLEW_OK)
				{
					std::cout << "GLEW initialized successfully" << std::endl;
				}
				else
				{
					std::cerr << "ERROR: GLEW failed to initialize \n" << glewGetErrorString(status) << std::endl;
				}
			}
			else
			{
				std::cerr << "ERROR: GLFW failed to initialize" << std::endl;
			}
		}
		virtual ~GameManager()
		{
			// Delete the display and clean up GLFW
			delete m_displayManager;
			glfwTerminate();
		}

		void start()
		{
			std::cout << "Game loop is now running" << std::endl;

			// START temporary data
			Loader loader;
			OBJLoader objloader;

			std::vector<Entity> entities;

			RawModel mBox = objloader.loadOBJ("tree.obj", loader);
			ModelTexture mtBox(loader.loadTexture("tree.png"));
			TexturedModel tmBox(mBox, mtBox);
			for (int i = 0; i < 25; i++) {
				int x = rand() % 500 - 250;
				int z = rand() % 500 - 250;
				entities.push_back(Entity(tmBox, glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
			}

			RawModel mGrass = objloader.loadOBJ("grassModel.obj", loader);
			ModelTexture mtGrass(loader.loadTexture("grassTexture.png"));
			TexturedModel tmGrass(mGrass, mtGrass);
			for (int i = 0; i < 100; i++) {
				int x = rand() % 500 - 250;
				int z = rand() % 500 - 250;
				entities.push_back(Entity(tmGrass, glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
			}

	

			TerrainTexture backgroundTexture = TerrainTexture(loader.loadTexture("grassy2.png"));
			TerrainTexture rTexture = TerrainTexture(loader.loadTexture("mud.png"));
			TerrainTexture gTexture = TerrainTexture(loader.loadTexture("grassFlowers.png"));
			TerrainTexture bTexture = TerrainTexture(loader.loadTexture("path.png"));
			TerrainTexture blendMap = TerrainTexture(loader.loadTexture("BlendMaps/blendMap.png"));

			TerrainTexturePack texturePack = TerrainTexturePack(backgroundTexture, rTexture, gTexture, bTexture);

			std::vector<Terrain> terrains;
			terrains.push_back(Terrain(0, -1, loader, texturePack, blendMap, "heightmap-perlin"));

			RawModel dragonM = objloader.loadOBJ("stanfordBunny.obj", loader);
			ModelTexture dragonT(loader.loadTexture("dragon.png"));
			dragonT.setShineDamper(10);
			dragonT.setReflectivity(1);
			TexturedModel dragon(dragonM, dragonT);
			Player player = Player(dragon, glm::vec3(0, 10, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
			//camera = Camera(player);

			// END temporary data

			MasterRenderer renderer = MasterRenderer();

			// Start the game loop
			while (m_displayManager->isWindowOpen())
			{
				processInput(glfwGetCurrentContext());
				//camera.Update(player);
				player.move();
				renderer.processEntity(player);
				for (Terrain& t : terrains)
					renderer.processTerrain(t);
				for (Entity& e : entities)
					renderer.processEntity(e);

				renderer.render(light, camera);

				m_displayManager->updateDisplay();
				m_displayManager->showFPS();
			}
		}

	private:
		DisplayManager* m_displayManager;
};

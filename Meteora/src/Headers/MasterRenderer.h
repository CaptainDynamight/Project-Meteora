#include "ShaderProgram.h"
#include "EntityRenderer.h"
#include "TerrainRenderer.h"
#include <vector>
#include <map>

class MasterRenderer
{
	public:
		MasterRenderer()
		{
			projectionMatrix = createProjectionMatrix();
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			renderer = new EntityRenderer(shader, projectionMatrix);
			terrainRenderer = new TerrainRenderer(terrainShader, projectionMatrix);
		}

		void cleanUp()
		{
			shader.cleanUp();
			terrainShader.cleanUp();
		}
		
		void render(Light sun, Camera camera)
		{
			prepare();
			shader.start();
			shader.loadSkyColor(RED, GREEN, BLUE);
			shader.loadLight(sun);
			shader.loadViewMatrix(camera);
			renderer->render(entities);
			shader.stop();
			terrainShader.start();
			terrainShader.loadSkyColor(RED, GREEN, BLUE);
			terrainShader.loadLight(sun);
			terrainShader.loadViewMatrix(camera);
			terrainRenderer->render(terrains);
			terrainShader.stop();
			terrains.clear();
			entities.clear();
		}

		void processTerrain(Terrain terrain)
		{
			terrains.push_back(terrain);
		}

		void processEntity(Entity& entity)
		{
			TexturedModel texturedModel = entity.getModel();
			entities.insert(std::map<TexturedModel, std::vector<Entity>, tmCompare>::value_type(texturedModel, std::vector<Entity>()));
			entities[texturedModel].push_back(entity);
		}

		void prepare()
		{
			glEnable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(RED, GREEN, BLUE, 1.0f);
		}

	private:
		ShaderProgram shader;
		EntityRenderer* renderer;
		TerrainRenderer* terrainRenderer;
		TerrainShader terrainShader = TerrainShader();
		std::vector<Terrain> terrains;
		std::map<TexturedModel, std::vector<Entity>, tmCompare> entities;

		const float FOV = 70;
		const float NEAR_PLANE = 0.1f;
		const float FAR_PLANE = 1000.0f;
		int SCR_WIDTH = 700;
		int	SCR_HEIGHT = 700;
		glm::mat4 projectionMatrix;
		float RED = 0.5f;
		float GREEN = 0.5f;
		float BLUE = 0.5f;

		glm::mat4 createProjectionMatrix()
		{
			return glm::perspective(FOV, (float)SCR_WIDTH / SCR_HEIGHT, NEAR_PLANE, FAR_PLANE);
		}
};

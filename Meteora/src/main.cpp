#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Headers\Loader.h"
#include "Headers\RawModel.h"
#include "Headers\ShaderProgram.h"
#include "Headers\ModelTexture.h"
#include "Headers\TexturedModel.h"
#include "Headers\Entity.h"
#include "Headers\Camera.h"
#include "Headers\OBJLoader.h"
#include "Headers\Light.h"
#include "Headers\MasterRenderer.h"
#include "Headers\Terrain.h"
#include "Headers\Terrain.h"
#include "Headers\TerrainTexture.h"
#include "Headers\TerrainTexturePack.h"

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(700, 700, "| METEORA |", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    int SCR_WIDTH, SCR_HEIGHT;
    glfwGetWindowSize(window, &SCR_WIDTH, &SCR_HEIGHT);

    if (glewInit() != GLEW_OK)
        std::cout << "Error" << std::endl;

    Loader loader;
    OBJLoader objloader;

    Light light = Light(glm::vec3(1000, 1000, 1000), glm::vec3(1, 1, 1));    
    Camera camera = Camera();

    TerrainTexture backgroundTexture = TerrainTexture(loader.loadTexture("grassy2.png"));
    TerrainTexture rTexture = TerrainTexture(loader.loadTexture("mud.png"));
    TerrainTexture gTexture = TerrainTexture(loader.loadTexture("grassFlowers.png"));
    TerrainTexture bTexture = TerrainTexture(loader.loadTexture("path.png"));
    TerrainTexture blendMap = TerrainTexture(loader.loadTexture("BlendMaps/blendMap.png"));

    TerrainTexturePack texturePack = TerrainTexturePack(backgroundTexture, rTexture, gTexture, bTexture);

    Terrain terrain1 = Terrain(0, -1, loader, texturePack, blendMap);
    Terrain terrain2 = Terrain(-1, -1, loader, texturePack, blendMap);
    std::vector<Entity> entities;


    RawModel mBox = objloader.loadOBJ("Tree.obj", loader);
    ModelTexture mtBox(loader.loadTexture("Tree.png"));
    TexturedModel tmBox(mBox, mtBox);
    for (int i = 0; i < 25; i++) {
        int x = rand() % 500 - 250;
        int z = rand() % 500 - 250;
        //entities.push_back(Entity(tmBox, glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)));
    }

    RawModel dragon = objloader.loadOBJ("dragon.obj", loader);
    ModelTexture dragonTexture(loader.loadTexture("dragon.png"));
    TexturedModel dragoon(dragon, dragonTexture);
    Entity dr = Entity(dragoon, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    entities.push_back(dr);
    MasterRenderer renderer = MasterRenderer();
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        camera.move();
        renderer.processTerrain(terrain1);
        renderer.processTerrain(terrain2);
        renderer.processEntity(dr);
        dr.increaseRotation(glm::vec3(0, 0.01, 0));
        renderer.render(light, camera);
        /* Poll for and process events */
        glfwPollEvents();
    }
    renderer.cleanUp();
    loader.cleanUp();
    glfwTerminate();
    return 0;
}
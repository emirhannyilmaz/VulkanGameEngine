#include "app.hpp"

void App::run() {
    Window* window = new Window(800, 600, "Vulkan Game Engine");
    Input::window = window->window;
    Input::sensitivity = 5.0f;
    Light* light = new Light(glm::vec3(0.0f, -10000.0f, -10000.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    PerspectiveCamera* perspectiveCamera = new PerspectiveCamera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 45.0f, (float) window->width / (float) window->height, 0.01f, 1000.0f, 200.0f);
    OrthographicCamera* orthographicCamera = new OrthographicCamera();
    Renderer* renderer = new Renderer(window, perspectiveCamera);
    EntityRenderer* entityRenderer = new EntityRenderer(renderer);
    TerrainRenderer* terrainRenderer = new TerrainRenderer(renderer);
    ShadowMapRenderer* shadowMapRenderer = new ShadowMapRenderer(renderer);
    SkyboxRenderer* skyboxRenderer = new SkyboxRenderer(renderer);
    WaterRenderer* waterRenderer = new WaterRenderer(renderer);
    renderer->entityRenderer = entityRenderer;
    renderer->shadowMapRenderer = shadowMapRenderer;
    renderer->terrainRenderer = terrainRenderer;
    renderer->skyboxRenderer = skyboxRenderer;
    renderer->waterRenderer = waterRenderer;
    
    Texture* terrainTexture = new Texture("res/textures/grass.png", 0.0f, 0.0f, renderer);
    Terrain* terrain = new Terrain(terrainTexture, "res/textures/terrain_heightmap.png", glm::vec2(0.0f, 0.0f), renderer);

    std::vector<Terrain*> terrains;
    terrains.push_back(terrain);

    Texture* skyboxTexture = new Texture({"res/textures/skybox_front.tga", "res/textures/skybox_back.tga", "res/textures/skybox_up.tga", "res/textures/skybox_down.tga", "res/textures/skybox_right.tga", "res/textures/skybox_left.tga"}, renderer);
    Skybox* skybox = new Skybox(skyboxTexture, 500.0f, renderer);

    WaterTile* waterTile = new WaterTile(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(100.0f, 100.0f), 0.6f, 20.0f, renderer);

    std::vector<WaterTile*> waterTiles;
    waterTiles.push_back(waterTile);

    std::vector<Entity*> entities;
/*
    ObjModelData treeModelData = ModelLoader::LoadObj("res/models/tree.obj");
    float treePreviousX = 0.0f;
    float treePreviousZ = 0.0f;
    for (int i = 0; i < 50; i++) {
        glm::vec3 treePosition;
        do {
            float x = std::rand() % 100;
            float z = std::rand() % 100;
            treePosition = glm::vec3(x, terrain->getHeightOfTerrain(x, z), z);
        } while((treePosition.y > waterTile->position.y) || std::abs(treePosition.x - treePreviousX) < 10.0f || std::abs(treePosition.z - treePreviousZ) < 10.0f);
        treePreviousX = treePosition.x;
        treePreviousZ = treePosition.z;

        Mesh* treeMesh = new Mesh(treeModelData.vertices, treeModelData.indices, renderer);
        Texture* treeTexture = new Texture("res/textures/tree.png", 0.0f, 0.0f, renderer);
        Entity* tree = new Entity(treeMesh, treeTexture, treePosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f), renderer);
        entities.push_back(tree);
    }
*/
    while (!glfwWindowShouldClose(window->window)) {
        glfwPollEvents();

        light->update(1000.0f, renderer->deltaTime);
        perspectiveCamera->update(20.0f, renderer->deltaTime);
        orthographicCamera->update(perspectiveCamera, light->viewMatrix);

        renderer->beginDrawing();

        renderer->beginRecordingCommands(renderer->offScreenCommandBuffers, false);

        renderer->beginRendering(renderer->shadowMapResources->renderPass, renderer->shadowMapResources->framebuffer, renderer->offScreenCommandBuffers, false);
        shadowMapRenderer->render(entities, light, perspectiveCamera, orthographicCamera, renderer->offScreenCommandBuffers);
        renderer->endRendering(renderer->offScreenCommandBuffers);

        renderer->beginRendering(renderer->waterResources->renderPass, renderer->waterResources->reflectionFramebuffer, renderer->offScreenCommandBuffers, true);
        perspectiveCamera->invert(2 * (std::abs(perspectiveCamera->position.y) - std::abs(waterTile->position.y)));
        terrainRenderer->render(terrains, light, perspectiveCamera, orthographicCamera, fogColor, glm::vec4(0.0f, -1.0f, 0.0f, waterTile->position.y + 1.0f), renderer->offScreenCommandBuffers, false);
        entityRenderer->render(entities, light, perspectiveCamera, fogColor, glm::vec4(0.0f, -1.0f, 0.0f, waterTile->position.y + 1.0f), renderer->offScreenCommandBuffers, false);
        skyboxRenderer->render(skybox, perspectiveCamera, fogColor, glm::vec4(0.0f, -1.0f, 0.0f, waterTile->position.y + 1.0f), renderer->offScreenCommandBuffers, false);
        perspectiveCamera->revert();
        renderer->endRendering(renderer->offScreenCommandBuffers);

        renderer->beginRendering(renderer->waterResources->renderPass, renderer->waterResources->refractionFramebuffer, renderer->offScreenCommandBuffers, true);
        terrainRenderer->render(terrains, light, perspectiveCamera, orthographicCamera, fogColor, glm::vec4(0.0f, 1.0f, 0.0f, -waterTile->position.y), renderer->offScreenCommandBuffers, false);
        entityRenderer->render(entities, light, perspectiveCamera, fogColor, glm::vec4(0.0f, 1.0f, 0.0f, -waterTile->position.y), renderer->offScreenCommandBuffers, false);
        skyboxRenderer->render(skybox, perspectiveCamera, fogColor, glm::vec4(0.0f, 1.0f, 0.0f, -waterTile->position.y), renderer->offScreenCommandBuffers, false);
        renderer->endRendering(renderer->offScreenCommandBuffers);

        renderer->endRecordingCommands(renderer->offScreenCommandBuffers, false);

        renderer->beginRecordingCommands(renderer->commandBuffers, true);

        renderer->beginRendering(renderer->renderPass, renderer->framebuffers[renderer->currentImageIndex], renderer->commandBuffers, true);
        terrainRenderer->render(terrains, light, perspectiveCamera, orthographicCamera, fogColor, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), renderer->commandBuffers, true);
        entityRenderer->render(entities, light, perspectiveCamera, fogColor, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), renderer->commandBuffers, true);
        skyboxRenderer->render(skybox, perspectiveCamera, fogColor, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), renderer->commandBuffers, true);
        waterRenderer->render(waterTiles, perspectiveCamera, light, fogColor, renderer->commandBuffers);
        renderer->endRendering(renderer->commandBuffers);

        renderer->endRecordingCommands(renderer->commandBuffers, true);

        renderer->endDrawing();
    }

    renderer->waitIdle();

    for (size_t i = 0; i < waterTiles.size(); i++) {
        delete waterTiles[i];
    }
    waterTiles.clear();

    delete skybox;

    for (size_t i = 0; i < entities.size(); i++) {
        delete entities[i];
    }
    entities.clear();

    for (size_t i = 0; i < terrains.size(); i++) {
        delete terrains[i];
    }
    terrains.clear();

    delete waterRenderer;
    delete skyboxRenderer;
    delete terrainRenderer;
    delete shadowMapRenderer;
    delete entityRenderer;
    delete renderer;
    delete orthographicCamera;
    delete perspectiveCamera;
    delete light;
    delete window;
}

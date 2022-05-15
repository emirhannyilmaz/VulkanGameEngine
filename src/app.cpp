#include "app.hpp"

void App::run() {
    Window* window = new Window(800, 600, "Vulkan Game Engine");
    Input::window = window->window;
    Input::sensitivity = 5.0f;
    PerspectiveCamera* perspectiveCamera = new PerspectiveCamera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 45.0f, (float) window->width / (float) window->height, 0.1f, 1000.0f);
    Light* light = new Light(glm::vec3(0.0f, -10000.0f, -10000.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    Renderer* renderer = new Renderer(window, perspectiveCamera);
    TerrainRenderer* terrainRenderer = new TerrainRenderer(renderer);
    EntityRenderer* entityRenderer = new EntityRenderer(renderer);
    SkyboxRenderer* skyboxRenderer = new SkyboxRenderer(renderer);
    WaterRenderer* waterRenderer = new WaterRenderer(renderer);
    renderer->terrainRenderer = terrainRenderer;
    renderer->entityRenderer = entityRenderer;
    renderer->skyboxRenderer = skyboxRenderer;
    renderer->waterRenderer = waterRenderer;
    
    Texture* terrainTexture = new Texture("res/textures/grass.png", 0.0f, 0.0f, renderer);
    Terrain* terrain = new Terrain(terrainTexture, "res/textures/terrain_heightmap.png", glm::vec2(0.0f, 0.0f), renderer);

    std::vector<Terrain*> terrains;
    terrains.push_back(terrain);

    ObjModelData carModelData = ModelLoader::LoadObj("res/models/car.obj");
    Mesh* carMesh = new Mesh(carModelData.vertices, carModelData.indices, renderer);
    Texture* carTexture = new Texture("res/textures/car.jpg", 1.0f, 10.0f, renderer);
    Entity* car = new Entity(carMesh, carTexture, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), renderer);

    std::vector<Entity*> entities;
    entities.push_back(car);

    Texture* skyboxTexture = new Texture({"res/textures/skybox_front.tga", "res/textures/skybox_back.tga", "res/textures/skybox_up.tga", "res/textures/skybox_down.tga", "res/textures/skybox_right.tga", "res/textures/skybox_left.tga"}, renderer);
    Skybox* skybox = new Skybox(skyboxTexture, 500.0f, renderer);

    WaterTile* waterTile = new WaterTile(glm::vec3(50.0f, 0.0f, 50.0f), glm::vec2(50.0f, 50.0f), 0.6f, 20.0f, renderer);

    std::vector<WaterTile*> waterTiles;
    waterTiles.push_back(waterTile);

    while (!glfwWindowShouldClose(window->window)) {
        glfwPollEvents();

        perspectiveCamera->update(40.0f, renderer->deltaTime);
        light->update(1000.0f, renderer->deltaTime);

        renderer->beginDrawing();

        renderer->beginRecordingCommands(renderer->offScreenCommandBuffers, false);

        renderer->beginRendering(renderer->waterResources->renderPass, renderer->waterResources->reflectionFramebuffer, renderer->offScreenCommandBuffers);
        perspectiveCamera->invert(2 * (std::abs(perspectiveCamera->position.y) - std::abs(waterTile->position.y)));
        terrainRenderer->render(terrains, light, perspectiveCamera, glm::vec4(0.0f, -1.0f, 0.0f, waterTile->position.y + 1.0f), renderer->offScreenCommandBuffers, false);
        entityRenderer->render(entities, light, perspectiveCamera, glm::vec4(0.0f, -1.0f, 0.0f, waterTile->position.y + 1.0f), renderer->offScreenCommandBuffers, false);
        skyboxRenderer->render(skybox, perspectiveCamera, glm::vec4(0.0f, -1.0f, 0.0f, waterTile->position.y + 1.0f), renderer->offScreenCommandBuffers, false);
        perspectiveCamera->revert();
        renderer->endRendering(renderer->offScreenCommandBuffers);

        renderer->beginRendering(renderer->waterResources->renderPass, renderer->waterResources->refractionFramebuffer, renderer->offScreenCommandBuffers);
        terrainRenderer->render(terrains, light, perspectiveCamera, glm::vec4(0.0f, 1.0f, 0.0f, -waterTile->position.y), renderer->offScreenCommandBuffers, false);
        entityRenderer->render(entities, light, perspectiveCamera, glm::vec4(0.0f, 1.0f, 0.0f, -waterTile->position.y), renderer->offScreenCommandBuffers, false);
        skyboxRenderer->render(skybox, perspectiveCamera, glm::vec4(0.0f, 1.0f, 0.0f, -waterTile->position.y), renderer->offScreenCommandBuffers, false);
        renderer->endRendering(renderer->offScreenCommandBuffers);

        renderer->endRecordingCommands(renderer->offScreenCommandBuffers, false);

        renderer->beginRecordingCommands(renderer->commandBuffers, true);

        renderer->beginRendering(renderer->renderPass, renderer->framebuffers[renderer->currentImageIndex], renderer->commandBuffers);
        terrainRenderer->render(terrains, light, perspectiveCamera, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), renderer->commandBuffers, true);
        entityRenderer->render(entities, light, perspectiveCamera, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), renderer->commandBuffers, true);
        skyboxRenderer->render(skybox, perspectiveCamera, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), renderer->commandBuffers, true);
        waterRenderer->render(waterTiles, perspectiveCamera, light, renderer->commandBuffers);
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
    delete entityRenderer;
    delete terrainRenderer;
    delete renderer;
    delete light;
    delete perspectiveCamera;
    delete window;
}

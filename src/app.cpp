#include "app.hpp"

void App::run() {
    Window* window = new Window(800, 600, "Vulkan Game Engine");
    Input::window = window->window;
    Input::sensitivity = 20.0f;
    Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 45.0f, (float) window->width / (float) window->height, 0.1f, 1000.0f);
    Light* light = new Light(glm::vec3(50.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    Renderer* renderer = new Renderer(window, camera);
    EntityRenderer* entityRenderer = new EntityRenderer(renderer);
    SkyboxRenderer* skyboxRenderer = new SkyboxRenderer(renderer);
    WaterRenderer* waterRenderer = new WaterRenderer(renderer);

    ObjModelData objModelData = ModelLoader::LoadObj("res/models/rocket.obj");

    Mesh* mesh = new Mesh(objModelData.vertices, objModelData.indices, renderer);
    Texture* texture = new Texture("res/textures/rocket.png", 1.0f, 10.0f, renderer);
    Entity* entity = new Entity(mesh, texture, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f), renderer);

    Mesh* mesh2 = new Mesh(objModelData.vertices, objModelData.indices, renderer);
    Texture* texture2 = new Texture("res/textures/rocket.png", 1.0f, 10.0f, renderer);
    Entity* entity2 = new Entity(mesh2, texture2, glm::vec3(-50.0f, 10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f), renderer);

    std::vector<Entity*> entities;
    entities.push_back(entity);
    entities.push_back(entity2);

    Texture* skyboxTexture = new Texture({"res/textures/skybox_front.tga", "res/textures/skybox_back.tga", "res/textures/skybox_up.tga", "res/textures/skybox_down.tga", "res/textures/skybox_right.tga", "res/textures/skybox_left.tga"}, renderer);
    Skybox* skybox = new Skybox(skyboxTexture, 500.0f, renderer);

    WaterTile* waterTile = new WaterTile(glm::vec3(0.0f, -10.0f, 0.0f), glm::vec2(25.0f, 25.0f), renderer);

    std::vector<WaterTile*> waterTiles;
    waterTiles.push_back(waterTile);

    while (!glfwWindowShouldClose(window->window)) {
        glfwPollEvents();

        camera->update(renderer->deltaTime);

        renderer->beginDrawing();

        float distance = 2 * (camera->position.y - waterTile->position.y);
        camera->position.y -= distance;
        camera->rotation.x *= -1.0f;
        renderer->beginRendering(renderer->waterResources->renderPass, renderer->waterResources->reflectionFramebuffer, renderer->offScreenCommandBuffers);
        entityRenderer->render(entities, light, camera, glm::vec4(0.0f, 1.0f, 0.0f, -waterTile->position.y), renderer->offScreenCommandBuffers);
        skyboxRenderer->render(skybox, camera, renderer->offScreenCommandBuffers);
        renderer->endRendering(renderer->offScreenCommandBuffers);
        camera->position.y += distance;
        camera->rotation.x *= -1.0f;

        renderer->beginRendering(renderer->waterResources->renderPass, renderer->waterResources->refractionFramebuffer, renderer->offScreenCommandBuffers);
        entityRenderer->render(entities, light, camera, glm::vec4(0.0f, -1.0f, 0.0f, waterTile->position.y), renderer->offScreenCommandBuffers);
        skyboxRenderer->render(skybox, camera, renderer->offScreenCommandBuffers);
        renderer->endRendering(renderer->offScreenCommandBuffers);

        renderer->beginRendering(renderer->renderPass, renderer->framebuffers[renderer->currentImageIndex], renderer->commandBuffers);
        entityRenderer->render(entities, light, camera, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), renderer->commandBuffers);
        skyboxRenderer->render(skybox, camera, renderer->commandBuffers);
        waterRenderer->render(waterTiles, camera, renderer->commandBuffers);
        renderer->endRendering(renderer->commandBuffers);

        renderer->endDrawing();
    }

    vkDeviceWaitIdle(renderer->device->device);

    for (size_t i = 0; i < waterTiles.size(); i++) {
        delete waterTiles[i];
    }
    waterTiles.clear();

    delete skybox;

    for (size_t i = 0; i < entities.size(); i++) {
        delete entities[i];
    }
    entities.clear();

    delete waterRenderer;
    delete skyboxRenderer;
    delete entityRenderer;
    delete renderer;
    delete light;
    delete camera;
    delete window;
}

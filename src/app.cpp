#include "app.hpp"

void App::run() {
    Window* window = new Window(800, 600, "Vulkan Game Engine");
    Input::window = window->window;
    Input::sensitivity = 5.0f;
    Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 45.0f, (float) window->width / (float) window->height, 0.1f, 1000.0f, 40.0f);
    Light* light = new Light(glm::vec3(50.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    Renderer* renderer = new Renderer(window, camera);
    EntityRenderer* entityRenderer = new EntityRenderer(renderer);
    SkyboxRenderer* skyboxRenderer = new SkyboxRenderer(renderer);
    WaterRenderer* waterRenderer = new WaterRenderer(renderer);
    renderer->waterRenderer = waterRenderer;

    ObjModelData carModelData = ModelLoader::LoadObj("res/models/car.obj");
    Mesh* carMesh = new Mesh(carModelData.vertices, carModelData.indices, renderer);
    Texture* carTexture = new Texture("res/textures/car.jpg", 1.0f, 10.0f, renderer);
    Entity* car = new Entity(carMesh, carTexture, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), renderer);

    std::vector<Entity*> entities;
    entities.push_back(car);

    Texture* skyboxTexture = new Texture({"res/textures/skybox_front.tga", "res/textures/skybox_back.tga", "res/textures/skybox_up.tga", "res/textures/skybox_down.tga", "res/textures/skybox_right.tga", "res/textures/skybox_left.tga"}, renderer);
    Skybox* skybox = new Skybox(skyboxTexture, 500.0f, renderer);

    WaterTile* waterTile = new WaterTile(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(25.0f, 25.0f), renderer);

    std::vector<WaterTile*> waterTiles;
    waterTiles.push_back(waterTile);

    while (!glfwWindowShouldClose(window->window)) {
        glfwPollEvents();

        camera->update(renderer->deltaTime);

        renderer->beginDrawing();
            renderer->beginRecordingCommands(renderer->offScreenCommandBuffers, false);
                renderer->beginRendering(renderer->waterResources->renderPass, renderer->waterResources->reflectionFramebuffer, renderer->offScreenCommandBuffers);
                    camera->invert(2 * (std::abs(camera->position.y) - std::abs(waterTile->position.y)));
                    entityRenderer->render(entities, light, camera, glm::vec4(0.0f, -1.0f, 0.0f, -waterTile->position.y), renderer->offScreenCommandBuffers, false);
                    skyboxRenderer->render(skybox, camera, glm::vec4(0.0f, -1.0f, 0.0f, -waterTile->position.y), renderer->offScreenCommandBuffers, false);
                    camera->revert();
                renderer->endRendering(renderer->offScreenCommandBuffers);

                renderer->beginRendering(renderer->waterResources->renderPass, renderer->waterResources->refractionFramebuffer, renderer->offScreenCommandBuffers);
                    entityRenderer->render(entities, light, camera, glm::vec4(0.0f, 1.0f, 0.0f, -waterTile->position.y), renderer->offScreenCommandBuffers, false);
                    skyboxRenderer->render(skybox, camera, glm::vec4(0.0f, 1.0f, 0.0f, -waterTile->position.y), renderer->offScreenCommandBuffers, false);
                renderer->endRendering(renderer->offScreenCommandBuffers);
            renderer->endRecordingCommands(renderer->offScreenCommandBuffers, false);

            renderer->beginRecordingCommands(renderer->commandBuffers, true);
                renderer->beginRendering(renderer->renderPass, renderer->framebuffers[renderer->currentImageIndex], renderer->commandBuffers);
                    entityRenderer->render(entities, light, camera, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), renderer->commandBuffers, true);
                    skyboxRenderer->render(skybox, camera, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), renderer->commandBuffers, true);
                    waterRenderer->render(waterTiles, camera, renderer->commandBuffers);
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

    delete waterRenderer;
    delete skyboxRenderer;
    delete entityRenderer;
    delete renderer;
    delete light;
    delete camera;
    delete window;
}

#include "app.hpp"

void App::run() {
    window = new Window(800, 600, "Vulkan Game Engine");
    Input::window = window->window;
    Input::sensitivity = 20.0f;
    camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 45.0f, (float) window->width / (float) window->height, 0.1f, 100.0f);
    light = new Light(glm::vec3(50.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    renderer = new Renderer(window, camera);

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

    while (!glfwWindowShouldClose(window->window)) {
        glfwPollEvents();

        camera->update(renderer->deltaTime);

        renderer->render(entities, light);
    }

    vkDeviceWaitIdle(renderer->device->device);

    for (size_t i = 0; i < entities.size(); i++) {
        delete entities[i];
    }
    entities.clear();
    delete renderer;
    delete light;
    delete camera;
    delete window;
}

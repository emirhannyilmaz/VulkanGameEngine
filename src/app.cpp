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
    Entity* entity = new Entity(mesh, texture, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f));

    while (!glfwWindowShouldClose(window->window)) {
        glfwPollEvents();

        camera->update(renderer->deltaTime);

        renderer->render(entity, light);
    }

    vkDeviceWaitIdle(renderer->device->device);

    delete entity;
    delete renderer;
    delete window;
}

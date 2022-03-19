#include "app.hpp"

void App::run() {
    window = new Window(800, 600, "Vulkan Game Engine");
    renderer = new Renderer(window);

    Entity* entity = new Entity(new Mesh(vertices, indices, renderer), glm::vec3(0.0f, 0.0f, 2.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    while (!glfwWindowShouldClose(window->window)) {
        glfwPollEvents();

        renderer->render(entity);
    }

    vkDeviceWaitIdle(renderer->device->device);

    delete entity;
    delete renderer;
    delete window;
}

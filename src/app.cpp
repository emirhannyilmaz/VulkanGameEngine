#include "app.hpp"

void App::run() {
    window = new Window(800, 600, "Vulkan Game Engine");
    Input::window = window->window;
    Input::sensitivity = 2.0f;
    camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 45.0f, (float) window->width / (float) window->height, 0.1f, 100.0f);
    renderer = new Renderer(window, camera);

    Entity* entity = new Entity(new Mesh(vertices, indices, renderer), glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    while (!glfwWindowShouldClose(window->window)) {
        glfwPollEvents();

        camera->update(renderer->deltaTime);

        renderer->render(entity);
    }

    vkDeviceWaitIdle(renderer->device->device);

    delete entity;
    delete renderer;
    delete window;
}

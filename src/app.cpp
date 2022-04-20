#include "app.hpp"

void App::run() {
    Window* window = new Window(800, 600, "Vulkan Game Engine");
    Input::window = window->window;
    Input::sensitivity = 20.0f;
    Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 45.0f, (float) window->width / (float) window->height, 0.1f, 100.0f);
    Light* light = new Light(glm::vec3(50.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    Renderer* renderer = new Renderer(window, camera);
    Texture* skyboxTexture = new Texture({"res/textures/skybox_right.jpg", "res/textures/skybox_left.jpg", "res/textures/skybox_top.jpg", "res/textures/skybox_bottom.jpg", "res/textures/skybox_back.jpg", "res/textures/skybox_front.jpg"}, renderer);
    Skybox* skybox = new Skybox(skyboxTexture, 500.0f, camera, renderer);

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

        renderer->render(entities, light, skybox);
    }

    vkDeviceWaitIdle(renderer->device->device);

    for (size_t i = 0; i < entities.size(); i++) {
        delete entities[i];
    }
    entities.clear();
    delete skybox;
    delete renderer;
    delete light;
    delete camera;
    delete window;
}

#include "app.hpp"

void App::run() {
    Window* window = new Window(800, 600, "Vulkan Game Engine");
    Input::window = window->window;
    Input::sensitivity = 0.2f;
    Light* light = new Light(glm::vec3(0.0f, -1000.0f, -1000.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    PerspectiveCamera* perspectiveCamera = new PerspectiveCamera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 45.0f, (float) window->width / (float) window->height, 0.01f, 1000.0f);
    OrthographicCamera* orthographicCamera = new OrthographicCamera();
    Renderer* renderer = new Renderer(window, perspectiveCamera);
    EntityRenderer* entityRenderer = new EntityRenderer(renderer);
    AnimatedEntityRenderer* animatedEntityRenderer = new AnimatedEntityRenderer(renderer);
    TerrainRenderer* terrainRenderer = new TerrainRenderer(renderer);
    EntityShadowMapRenderer* entityShadowMapRenderer = new EntityShadowMapRenderer(renderer);
    AnimatedEntityShadowMapRenderer* animatedEntityShadowMapRenderer = new AnimatedEntityShadowMapRenderer(renderer);
    SkyboxRenderer* skyboxRenderer = new SkyboxRenderer(renderer);
    WaterRenderer* waterRenderer = new WaterRenderer(renderer);
    ParticleRenderer* particleRenderer = new ParticleRenderer(renderer);
    renderer->entityRenderer = entityRenderer;
    renderer->animatedEntityRenderer = animatedEntityRenderer;
    renderer->entityShadowMapRenderer = entityShadowMapRenderer;
    renderer->animatedEntityShadowMapRenderer = animatedEntityShadowMapRenderer;
    renderer->terrainRenderer = terrainRenderer;
    renderer->skyboxRenderer = skyboxRenderer;
    renderer->waterRenderer = waterRenderer;
    renderer->particleRenderer = particleRenderer;
    
    Texture* terrainTexture = new Texture("res/textures/grass.png", 0.0f, 0.0f, renderer);
    Terrain* terrain = new Terrain(terrainTexture, "res/textures/terrain_heightmap.png", glm::vec2(0.0f, 0.0f), renderer);

    std::vector<Terrain*> terrains;
    terrains.push_back(terrain);

    Texture* skyboxTexture = new Texture({"res/textures/skybox_front.tga", "res/textures/skybox_back.tga", "res/textures/skybox_up.tga", "res/textures/skybox_down.tga", "res/textures/skybox_right.tga", "res/textures/skybox_left.tga"}, renderer);
    Skybox* skybox = new Skybox(skyboxTexture, 500.0f, renderer);

    WaterTile* waterTile = new WaterTile(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(800.0f, 800.0f), 0.6f, 20.0f, renderer);

    std::vector<WaterTile*> waterTiles;
    waterTiles.push_back(waterTile);

    std::vector<Entity*> entities;

    MeshData treeMeshData = ColladaLoader::LoadMesh("res/models/tree.dae");
    for (size_t i = 0; i < 100; i++) {
        glm::vec3 treePosition;
        do {
            float x = std::rand() % 800;
            float z = std::rand() % 800;
            treePosition = glm::vec3(x, terrain->getHeightOfTerrain(x, z), z);
        } while(!checkIfTreePositionIsSuitable(entities, treePosition, waterTile->position));
        Entity* tree = new Entity(new EntityMesh(treeMeshData.vertices, treeMeshData.indices, renderer), new Texture("res/textures/tree.png", 0.0f, 0.0f, renderer), treePosition, glm::vec3(180.0f, 0.0f, 0.0f), glm::vec3(8.0f, 8.0f, 8.0f), renderer);
        entities.push_back(tree);
    }

    std::vector<AnimatedEntity*> animatedEntities;

    AnimatedMeshData characterMeshData = ColladaLoader::LoadAnimatedMesh("res/models/character.dae");
    AnimationData characterAnimationData = ColladaLoader::LoadAnimation("res/models/character.dae");
    AnimatedEntity* character = new AnimatedEntity(new AnimatedEntityMesh(characterMeshData.vertices, characterMeshData.indices, renderer), new Texture("res/textures/character.png", 0.0f, 0.0f, renderer), characterAnimationData.rootJoint, characterAnimationData.jointCount, glm::vec3(20.0f, 0.0f, 20.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), renderer);
    animatedEntities.push_back(character);

    Animator* characterAnimator = new Animator(character);
    characterAnimator->setAnimation(&characterAnimationData.animation);

    ParticleSystem* particleSystem = new ParticleSystem("res/textures/star.png", 1, 150, 400.0f, 3.0f, 0.1f, renderer);

    std::vector<Particle*> particles;
    std::vector<Particle*> particlesToDelete;
    
    while (!glfwWindowShouldClose(window->window)) {
        glfwPollEvents();

        light->update(1000.0f, renderer->deltaTime);
        perspectiveCamera->update(400.0f, renderer->deltaTime);
        orthographicCamera->update(perspectiveCamera, light->viewMatrix);
        characterAnimator->update(7.0f, renderer->deltaTime);

        for (size_t i = 0; i < particlesToDelete.size(); i++) {
            if (particlesToDelete[i]->deleteAtFrame.has_value()) {
                if (particlesToDelete[i]->deleteAtFrame.value() == renderer->currentFrame) {
                    delete particlesToDelete[i];
                    particlesToDelete.erase(particlesToDelete.begin() + i);
                }
            }
        }

        for (size_t i = 0; i < particles.size(); i++) {
            if (!particles[i]->update(renderer->deltaTime, renderer->realDeltaTime)) {
                particles[i]->deleteAtFrame = renderer->currentFrame;
                particlesToDelete.push_back(particles[i]);
                particles.erase(particles.begin() + i);
            }
        }

        particleSystem->emit(glm::vec3(0.0f, 0.0f, 0.0f), particles);

        renderer->beginDrawing();

        renderer->beginRecordingCommands(renderer->offScreenCommandBuffers);

        renderer->beginRendering(renderer->shadowMapResources->renderPass, renderer->shadowMapResources->framebuffer, renderer->offScreenCommandBuffers, false);
        entityShadowMapRenderer->render(entities, light, perspectiveCamera, orthographicCamera, renderer->offScreenCommandBuffers);
        animatedEntityShadowMapRenderer->render(animatedEntities, light, perspectiveCamera, orthographicCamera, renderer->offScreenCommandBuffers);
        renderer->endRendering(renderer->offScreenCommandBuffers);

        renderer->beginRendering(renderer->waterResources->renderPass, renderer->waterResources->reflectionFramebuffer, renderer->offScreenCommandBuffers, true);
        perspectiveCamera->invert(2 * (std::abs(perspectiveCamera->position.y) - std::abs(waterTile->position.y)));
        terrainRenderer->render(terrains, light, perspectiveCamera, orthographicCamera, glm::vec4(0.0f, -1.0f, 0.0f, waterTile->position.y + 1.0f), renderer->offScreenCommandBuffers, false);
        entityRenderer->render(entities, light, perspectiveCamera, glm::vec4(0.0f, -1.0f, 0.0f, waterTile->position.y + 1.0f), renderer->offScreenCommandBuffers, false);
        animatedEntityRenderer->render(animatedEntities, light, perspectiveCamera, glm::vec4(0.0f, -1.0f, 0.0f, waterTile->position.y + 1.0f), renderer->offScreenCommandBuffers, false);
        skyboxRenderer->render(skybox, perspectiveCamera, glm::vec4(0.0f, -1.0f, 0.0f, waterTile->position.y + 1.0f), renderer->offScreenCommandBuffers, false);
        particleRenderer->render(particles, perspectiveCamera, glm::vec4(0.0f, -1.0f, 0.0f, waterTile->position.y + 1.0f), renderer->offScreenCommandBuffers, false);
        perspectiveCamera->revert();
        renderer->endRendering(renderer->offScreenCommandBuffers);

        renderer->beginRendering(renderer->waterResources->renderPass, renderer->waterResources->refractionFramebuffer, renderer->offScreenCommandBuffers, true);
        terrainRenderer->render(terrains, light, perspectiveCamera, orthographicCamera, glm::vec4(0.0f, 1.0f, 0.0f, -waterTile->position.y), renderer->offScreenCommandBuffers, false);
        entityRenderer->render(entities, light, perspectiveCamera, glm::vec4(0.0f, 1.0f, 0.0f, -waterTile->position.y), renderer->offScreenCommandBuffers, false);
        animatedEntityRenderer->render(animatedEntities, light, perspectiveCamera, glm::vec4(0.0f, -1.0f, 0.0f, waterTile->position.y + 1.0f), renderer->offScreenCommandBuffers, false);
        skyboxRenderer->render(skybox, perspectiveCamera, glm::vec4(0.0f, 1.0f, 0.0f, -waterTile->position.y), renderer->offScreenCommandBuffers, false);
        particleRenderer->render(particles, perspectiveCamera, glm::vec4(0.0f, 1.0f, 0.0f, -waterTile->position.y), renderer->offScreenCommandBuffers, false);
        renderer->endRendering(renderer->offScreenCommandBuffers);

        renderer->endRecordingCommands(renderer->offScreenCommandBuffers);

        renderer->beginRecordingCommands(renderer->commandBuffers);

        renderer->beginRendering(renderer->renderPass, renderer->framebuffers[renderer->currentImageIndex], renderer->commandBuffers, true);
        terrainRenderer->render(terrains, light, perspectiveCamera, orthographicCamera, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), renderer->commandBuffers, true);
        entityRenderer->render(entities, light, perspectiveCamera, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), renderer->commandBuffers, true);
        animatedEntityRenderer->render(animatedEntities, light, perspectiveCamera, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), renderer->commandBuffers, true);
        skyboxRenderer->render(skybox, perspectiveCamera, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), renderer->commandBuffers, true);
        waterRenderer->render(waterTiles, perspectiveCamera, light, renderer->commandBuffers);
        particleRenderer->render(particles, perspectiveCamera, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), renderer->commandBuffers, true);
        renderer->endRendering(renderer->commandBuffers);

        renderer->endRecordingCommands(renderer->commandBuffers);

        renderer->endDrawing();
    }

    renderer->waitIdle();

    for (size_t i = 0; i < particlesToDelete.size(); i++) {
        delete particlesToDelete[i];
    }
    particlesToDelete.clear();

    for (size_t i = 0; i < particles.size(); i++) {
        delete particles[i];
    }
    particles.clear();

    delete particleSystem;
    delete characterAnimator;

    for (size_t i = 0; i < animatedEntities.size(); i++) {
        delete animatedEntities[i];
    }
    animatedEntities.clear();

    for (size_t i = 0; i < entities.size(); i++) {
        delete entities[i];
    }
    entities.clear();

    for (size_t i = 0; i < waterTiles.size(); i++) {
        delete waterTiles[i];
    }
    waterTiles.clear();

    delete skybox;

    for (size_t i = 0; i < terrains.size(); i++) {
        delete terrains[i];
    }
    terrains.clear();

    delete particleRenderer;
    delete waterRenderer;
    delete skyboxRenderer;
    delete terrainRenderer;
    delete animatedEntityShadowMapRenderer;
    delete entityShadowMapRenderer;
    delete animatedEntityRenderer;
    delete entityRenderer;
    renderer->entityRenderer = nullptr;
    renderer->animatedEntityRenderer = nullptr;
    renderer->entityShadowMapRenderer = nullptr;
    renderer->animatedEntityShadowMapRenderer = nullptr;
    renderer->terrainRenderer = nullptr;
    renderer->skyboxRenderer = nullptr;
    renderer->waterRenderer = nullptr;
    delete renderer;
    delete orthographicCamera;
    delete perspectiveCamera;
    delete light;
    delete window;
}

bool App::checkIfTreePositionIsSuitable(std::vector<Entity*>& entities, glm::vec3 treePosition, glm::vec3 waterPosition) {
    if (treePosition.y > waterPosition.y) {
        return false;
    }

    for (Entity* entity : entities) {
        if (glm::length(treePosition - entity->position) < 30.0f) {
            return false;
        }
    }

    return true;
}

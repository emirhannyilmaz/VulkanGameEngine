#ifndef app_hpp
#define app_hpp

#include "io/window.hpp"
#include "rendering/renderer/renderer.hpp"
#include "rendering/entity_renderer/entity_renderer.hpp"
#include "rendering/animated_entity_renderer/animated_entity_renderer.hpp"
#include "rendering/shadow_map_renderer/entity_shadow_map_renderer.hpp"
#include "rendering/shadow_map_renderer/animated_entity_shadow_map_renderer.hpp"
#include "rendering/terrain_renderer/terrain_renderer.hpp"
#include "rendering/skybox_renderer/skybox_renderer.hpp"
#include "rendering/water_renderer/water_renderer.hpp"
#include "rendering/particle_renderer/particle_renderer.hpp"
#include "rendering/post_processing/horizontal_gaussian_blur_post_processing.hpp"
#include "rendering/post_processing/vertical_gaussian_blur_post_processing.hpp"
#include "utils/collada_loader.hpp"
#include "animation/animator.hpp"
#include "particle/particle_system.hpp"
#include "post_processing_manager.hpp"

class App {
public:
    void run();
private:
    bool checkIfTreePositionIsSuitable(std::vector<Entity*>& entities, glm::vec3 treePosition, glm::vec3 waterPosition);
};

#endif

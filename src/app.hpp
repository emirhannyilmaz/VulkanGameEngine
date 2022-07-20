#ifndef app_hpp
#define app_hpp

#include "io/window.hpp"
#include "rendering/renderer/renderer.hpp"
#include "rendering/entity_renderer/entity_renderer.hpp"
#include "rendering/animated_entity_renderer/animated_entity_renderer.hpp"
#include "rendering/shadow_map_renderer/shadow_map_renderer.hpp"
#include "rendering/terrain_renderer/terrain_renderer.hpp"
#include "rendering/skybox_renderer/skybox_renderer.hpp"
#include "rendering/water_renderer/water_renderer.hpp"
#include "utils/collada_loader.hpp"
#include "animation/animator.hpp"

class App {
public:
    void run();
};

#endif

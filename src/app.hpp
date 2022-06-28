#ifndef app_hpp
#define app_hpp

#include "io/window.hpp"
#include "rendering/renderer/renderer.hpp"
#include "rendering/entity_renderer/entity_renderer.hpp"
#include "rendering/shadow_map_renderer/shadow_map_renderer.hpp"
#include "rendering/terrain_renderer/terrain_renderer.hpp"
#include "rendering/skybox_renderer/skybox_renderer.hpp"
#include "rendering/water_renderer/water_renderer.hpp"

class App {
public:
    void run();
};

#endif

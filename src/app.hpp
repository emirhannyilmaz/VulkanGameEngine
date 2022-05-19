#ifndef app_hpp
#define app_hpp

#include "io/window.hpp"
#include "rendering/renderer.hpp"
#include "rendering/entity_renderer.hpp"
#include "rendering/shadow_map_renderer.hpp"
#include "rendering/terrain_renderer.hpp"
#include "rendering/skybox_renderer.hpp"
#include "rendering/water_renderer.hpp"
#include "rendering/model_loader.hpp"

class App {
public:
    void run();
};

#endif

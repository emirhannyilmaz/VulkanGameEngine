#ifndef cubemap_hpp
#define cubemap_hpp

#include <stb/stb_image.h>
#include <string>
#include "renderer.hpp"
#include "image.hpp"
#include "sampler.hpp"
#include "buffer.hpp"

class Cubemap {
public:
    Image* image;
    Sampler* sampler;
    Cubemap(const std::vector<std::string>& fileNames, Renderer* renderer);
    ~Cubemap();
};

#endif

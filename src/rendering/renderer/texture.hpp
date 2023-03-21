#ifndef texture_hpp
#define texture_hpp

#include <stb/stb_image.h>
#include <string>
#include "renderer.hpp"
#include "image.hpp"
#include "sampler.hpp"
#include "buffer.hpp"

class Texture {
public:
    Image* image;
    Sampler* sampler;
    float reflectivity;
    float shineDamper;
    Texture(const std::string& fileName, float reflectivity, float shineDamper, Renderer* renderer);
    ~Texture();
};

#endif

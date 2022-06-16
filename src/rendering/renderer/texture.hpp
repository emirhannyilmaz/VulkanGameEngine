#ifndef texture_hpp
#define texture_hpp

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
    Texture(const std::vector<std::string>& fileNames, Renderer* renderer);
    ~Texture();
};

#endif

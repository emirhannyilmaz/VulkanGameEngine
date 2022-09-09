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
    int rowCount;
    Texture(const std::string& fileName, float reflectivity, float shineDamper, Renderer* renderer);
    Texture(const std::vector<std::string>& fileNames, Renderer* renderer);
    Texture(const std::string& fileName, int rowCount, Renderer* renderer);
    ~Texture();
};

#endif

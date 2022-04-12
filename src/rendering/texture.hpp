#ifndef texture_hpp
#define texture_hpp

#include <string>
#include "renderer.hpp"
#include "image.hpp"

class Texture {
public:
    Image* image;
    float reflectivity;
    float shineDamper;
    Texture(const std::string& fileName, float reflectivity, float shineDamper, Renderer* renderer);
    ~Texture();
};

#endif

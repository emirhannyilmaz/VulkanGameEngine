#ifndef texture_hpp
#define texture_hpp

#define STB_IMAGE_IMPLEMENTATION
#include "../tools/stb_image.h"

#include <string>
#include "renderer.hpp"
#include "image.hpp"

class Texture {
public:
    Image* image;
    Texture(const std::string& fileName, Renderer* renderer);
    ~Texture();
};

#endif

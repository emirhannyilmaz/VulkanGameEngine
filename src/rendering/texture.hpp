#ifndef texture_hpp
#define texture_hpp

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

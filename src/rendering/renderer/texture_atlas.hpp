#ifndef texture_atlas_hpp
#define texture_atlas_hpp

#include <stb_image.h>
#include <string>
#include "renderer.hpp"
#include "image.hpp"
#include "sampler.hpp"
#include "buffer.hpp"

class TextureAtlas {
public:
    Image* image;
    Sampler* sampler;
    int rowCount;
    int imageCount;
    TextureAtlas(const std::string& fileName, int rowCount, Renderer* renderer);
    ~TextureAtlas();
    glm::vec2 getOffsetAtIndex(int index);
};

#endif

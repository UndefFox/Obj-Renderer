#include <iostream>

#include "graphics/renderEngine.h"
#include "shapeGenerator.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"


int main() {
    int size = 512;

    RenderEngine engine{};

    engine.initialize();

    engine.setVetecies(ShapeGenerator::box(2.0f, 1.0f, 1.0f));
    engine.setCamera(30.0f, 45.0f, 2.0f, -0.2f, 90.0f);
    engine.setImage(size, {0, 0, 0, 0});

    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load("./box_example.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    std::vector<std::uint8_t> pixelsVec;
    pixelsVec.assign(reinterpret_cast<std::uint8_t*>(pixels), reinterpret_cast<std::uint8_t*>(pixels) + texWidth * texHeight * 4);


    engine.setTexture(pixelsVec, texWidth, texHeight);

    engine.render();

    stbi_write_bmp("./output.bmp", size, size, 4, engine.getImage().data());

    engine.destroy();

    return 0;
}
#include <iostream>

#include "graphics/renderEngine.h"
#include "fileController.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"

int main() {
    std::vector<Vertex> vertices = FileController::readObjFile("./box.obj");

    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load("./box_example.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    std::vector<std::uint8_t> pixelsVec;
    pixelsVec.assign(reinterpret_cast<std::uint8_t*>(pixels), reinterpret_cast<std::uint8_t*>(pixels) + texWidth * texHeight * 4);

    RenderParametrs params {
        .imageHeight = 512,
        .imageWidth = 512,

        .baseColorTextureHeight = texHeight,
        .baseColorTextureWidth = texWidth,
        .baseColorTexture = pixelsVec.data(),

        .cameraHeight = -0.2f,
        .cameraPitch = 30.0f,
        .cameraYaw = 45.0f,
        .cameraDistance = 2.0f,
        .cameraFOV = 90.0f,

        .modelVertices = vertices.data(),
        .modelVerticesCount = vertices.size()
    };
    
    RenderEngine engine(params);

    engine.render();

    stbi_write_png("./output.png", params.imageWidth, params.imageHeight, 4, engine.getImage().data(), 0);


    return 0;
}
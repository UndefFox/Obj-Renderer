#include <iostream>

#include "graphics/renderEngine.h"
#include "fileController.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"

int main() {
    RenderParametrs params{};
    params.imageHeight = 1024;
    params.imageWidth = 1024;

    params.cameraHeight = -0.1f;
    params.cameraPitch = 30.0f;
    params.cameraYaw = 25.0f;
    params.cameraDistance = 2.0f;
    params.cameraFOV = 90.0f;

    params.baseColorTexture = stbi_load("./box_example.png", &params.baseColorTextureWidth, &params.baseColorTextureHeight, nullptr, STBI_rgb_alpha);

    params.fontColorTexture = stbi_load("./resources/font_bitmap.png", &params.fontColorTextureWidth, &params.fontColorTextureHeight, nullptr, STBI_rgb_alpha);

    std::vector<Vertex> vertices = FileController::readObjFile("./box.obj");
    params.modelVertices = vertices.data();
    params.modelVerticesCount = vertices.size();
    
    RenderEngine engine(params);

    engine.render();

    stbi_write_png("./output.png", params.imageWidth, params.imageHeight, 4, engine.getImage().data(), 0);


    return 0;
}
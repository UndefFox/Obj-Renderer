#pragma once

#include "graphics/vertex.h"

struct RenderParametrs {
public:
    unsigned int imageHeight;
    unsigned int imageWidth;

    float backgroundColor[4];

    int baseColorTextureHeight;
    int baseColorTextureWidth;
    std::uint8_t *baseColorTexture;

    int fontColorTextureHeight;
    int fontColorTextureWidth;
    std::uint8_t *fontColorTexture;

    float cameraHeight;
    float cameraPitch;
    float cameraYaw;
    float cameraDistance;
    float cameraFOV;

    Vertex *modelVertices;
    unsigned int modelVerticesCount;
};

#pragma once

#include "graphics/vertex.h"

struct RenderParametrs {
public:
    unsigned int imageHeight;
    unsigned int imageWidth;

    float backgroundColor[4];

    unsigned int baseColorTextureHeight;
    unsigned int baseColorTextureWidth;
    std::uint8_t *baseColorTexture;

    float cameraHeight;
    float cameraPitch;
    float cameraYaw;
    float cameraDistance;
    float cameraFOV;

    Vertex *modelVertices;
    unsigned int modelVerticesCount;
};

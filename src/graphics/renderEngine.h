#pragma once

#include <vector>

#include "graphics/vertex.h"

class PrivateRenderEngine;


class RenderEngine {
private:
    PrivateRenderEngine* privateRenderEngine;

    std::uint32_t vertexCount;
    std::uint32_t renderWidht;
    std::uint32_t renderHeight;
    std::array<uint8_t, 4> renderBackgroundColor;

public:
    RenderEngine();
    ~RenderEngine();

    void initialize();
    void destroy();

    void setVetecies(const std::vector<Vertex> &vertices);
    /**
     * Sets new position of the camera.
     * 
     * @param pitch Pitch of camera in degrees. 0: looks forward.
     * @param yaw Pitch of camera in degrees. 0: looks in -X diretion .
     * @param distance How far camera from the point that camera is focused on.
     * @param height Height of the point that camera is focused on.
     * @param fox Angle of view in degrees.
     */
    void setCamera(float pitch, float yaw, float distance, float height, float fov);
    void setImage(std::uint32_t imageSize, std::array<uint8_t, 4> backgroundColor);
    void setTexture(std::vector<std::uint8_t>, std::uint32_t width, std::uint32_t height);

    void render();
    std::vector<std::uint8_t> getImage();
};
#pragma once

#include "graphics/renderDevice.h"
#include "graphics/renderHelpers.h"

#include "graphics/vertex.h"
#include "graphics/renderParametrs.h"


class RenderResources {
public:
    std::uint32_t imageHeight;
    std::uint32_t imageWidth;
    VkClearColorValue backgroundColor;

    VkBuffer modelVertices;
    VkDeviceMemory modelVerticesMemory;
    std::uint32_t modelVerticesCount;

    VkBuffer lineVertices;
    VkDeviceMemory lineVerticesMemory;
    std::uint32_t lineVerticesCount;

    VkBuffer uboCamera;
    VkDeviceMemory uboCameraMemory;

    VkImage renderImage;
    VkDeviceMemory renderImageMemory;
    VkImageView renderImageView;

    VkImage baseColor;
    VkDeviceMemory baseColorMemory;
    VkImageView baseColorView;

    VkImage fontColor;
    VkDeviceMemory fontColorMemory;
    VkImageView fontColorView;

    VkSampler defaultSampler;

private:
    RenderDevice &renderDevice;
    RenderHelpers &renderHelpers;

public:
    RenderResources() = delete;
    RenderResources(const RenderParametrs &parametrs, RenderDevice &device, RenderHelpers &helpers);
    ~RenderResources();

   

private:
    void initializeModelVertices(const Vertex *vertices, unsigned int count);
    void destroyModelVertices();

    void initializeLineVertices(const Vertex *vertices, unsigned int count, float pitch, float yaw, float distance, float height);
    void destroyLineVertices();

    void initializeUBOCamera(float pitch, float yaw, float distance, float height, float fov, float aspectRatio);
    void destroyUBOCamera();

    void initializeRenderImage(std::uint32_t width, std::uint32_t height);
    void destroyRenderImage();

    void initializeBaseColorTexture(const std::uint8_t *image, std::uint32_t width, std::uint32_t height);
    void destroyBaseColorTexture();

    void initializeFontColorTexture(const std::uint8_t *image, std::uint32_t width, std::uint32_t height);
    void destroyFontColorTexture();

    void initializeDefaultSempler();
    void destroyDefaultSampler();

    
};
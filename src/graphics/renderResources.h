#pragma once

#include "graphics/renderDevice.h"
#include "graphics/vertex.h"
#include "graphics/renderParametrs.h"


class RenderResources {
public:
    VkBuffer modelVertices;
    VkDeviceMemory modelVerticesMemory;

    VkBuffer uboCamera;
    VkDeviceMemory uboCameraMemory;

    VkImage renderImage;
    VkDeviceMemory renderImageMemory;
    VkImageView renderImageView;

    VkImage baseColor;
    VkDeviceMemory baseColorMemory;
    VkImageView baseColorView;

    VkSampler defaultSampler;

private:
    const RenderDevice &renderDevice;

public:
    RenderResources() = delete;
    RenderResources(const RenderParametrs &parametrs, RenderDevice &device);
    ~RenderResources();

    std::uint32_t findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

private:
    void initializeModelVertices(const Vertex *vertices, unsigned int count);
    void destroyModelVertices();

    void initializeUBOCamera(float pitch, float yaw, float distance, float height, float fov, float aspectRatio);
    void destroyUBOCamera();

    void initializeRenderImage(std::uint32_t width, std::uint32_t height);
    void destroyRenderImage();

    void initializeBaseColorTexture(const std::uint8_t *image, std::uint32_t width, std::uint32_t height);
    void destroyBaseColorTexture();

    void initializeDefaultSempler();
    void destroyDefaultSampler();

    
};
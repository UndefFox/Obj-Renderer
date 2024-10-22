#pragma once

#include "graphics/renderDevice.h"

#include <string>
#include <vulkan/vulkan.h>

class RenderHelpers {
private:
    RenderDevice &renderDevice;

public:
    RenderHelpers() = delete;
    RenderHelpers(RenderDevice &device);
    ~RenderHelpers();

    std::uint32_t findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

    VkShaderModule createShaderModule(const std::string filePath);

    void transitImageLayout(VkImage &image, VkImageLayout oldLayout, VkImageLayout newLayout, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask);
    void writeToImage(VkImage &image, VkImageLayout imageLayout, const std::uint8_t *data, unsigned int width, unsigned int height);
    void getImage(VkImage &image, VkImageLayout imageLayout, std::uint8_t *data, unsigned int width, unsigned int height);
};
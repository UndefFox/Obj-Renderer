#pragma once

#include "graphics/renderDevice.h"

#include <string>
#include <vector>
#include <vulkan/vulkan.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

    std::vector<Vertex> getTextGeometry(std::string text, glm::vec3 topCorner, glm::vec3 direction, glm::vec3 downDirection, float scale, bool mirror, bool bottomAligment);
};
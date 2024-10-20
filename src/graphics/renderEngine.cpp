#include "./renderEngine.h"

#include <array>
#include <cstring>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics/vertex.h"
#include "graphics/uboCamera.h"

RenderEngine::RenderEngine(const RenderParametrs &params) : 
    parametrs(params),
    device(parametrs),
    resources(parametrs, device),
    colorPass(parametrs, device, resources)
{}

RenderEngine::~RenderEngine() { }


void RenderEngine::render() {
    colorPass.render(parametrs);
}

std::vector<std::uint8_t> RenderEngine::getImage() {
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = resources.renderImage;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = 0;

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(device.universalBuffer, &beginInfo);

    vkCmdPipelineBarrier(
        device.universalBuffer,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    vkEndCommandBuffer(device.universalBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &device.universalBuffer;

    vkQueueSubmit(device.universalQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(device.universalQueue);

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    std::size_t memSize = sizeof(std::uint8_t) * parametrs.imageHeight * parametrs.imageWidth * 4;

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = memSize;
    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    vkCreateBuffer(device.logicalDevice, &bufferInfo, nullptr, &stagingBuffer);

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device.logicalDevice, resources.renderImage, &memRequirements);

    vkGetBufferMemoryRequirements(device.logicalDevice, stagingBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = resources.findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    vkAllocateMemory(device.logicalDevice, &allocInfo, nullptr, &stagingBufferMemory);

    vkBindBufferMemory(device.logicalDevice, stagingBuffer, stagingBufferMemory, 0);

    vkBeginCommandBuffer(device.universalBuffer, &beginInfo);

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
        parametrs.imageWidth,
        parametrs.imageHeight,
        1
    };

    vkCmdCopyImageToBuffer(
        device.universalBuffer,
        resources.renderImage,
        VK_IMAGE_LAYOUT_GENERAL,
        stagingBuffer,
        1,
        &region
    );

    vkEndCommandBuffer(device.universalBuffer);

    vkQueueSubmit(device.universalQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(device.universalQueue);

    std::size_t count = parametrs.imageWidth * parametrs.imageHeight * 4;

    std::vector<std::uint8_t> pixels(count);
    void *data;
    vkMapMemory(device.logicalDevice, stagingBufferMemory, 0, sizeof(std::uint8_t) * count, 0, &data);
        std::memcpy(pixels.data(), data, sizeof(std::uint8_t) * count);
    vkUnmapMemory(device.logicalDevice, stagingBufferMemory);

    vkDestroyBuffer(device.logicalDevice, stagingBuffer, nullptr);
    vkFreeMemory(device.logicalDevice, stagingBufferMemory, nullptr);

    return pixels;
}
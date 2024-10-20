#pragma once

#include "graphics/renderParametrs.h"

#include <vulkan/vulkan.h>
#include <cstdint>


class RenderDevice {
public:
    VkInstance vulkanInstance;
    VkPhysicalDevice physicalDevice;
    VkDevice logicalDevice;

    std::uint32_t universalQueueIndex;
    VkQueue universalQueue;

    VkCommandPool universalPool;
    VkCommandBuffer universalBuffer;

public:
    RenderDevice() = delete;
    RenderDevice(const RenderParametrs &parametrs);
    ~RenderDevice();

private:
    void initializeVulkanInstance();
    void destroyVulkanInstance();
    
    void initializePhysicalDevice();
    
    void initializeLogicalDevice();
    void destroyLogicalDevice();

    void initializeQueues();

    void initializeCommandBuffers();
    void destroyCommandBuffers();
};
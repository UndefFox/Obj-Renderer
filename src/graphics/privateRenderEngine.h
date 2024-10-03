#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

#include "graphics/vertex.h"
#include "graphics/ubo.h"

class PrivateRenderEngine {
private:
    VkInstance vulkanInstance;

    VkPhysicalDevice physicalDevice;

    VkDevice renderDevice;
    std::uint32_t queueFamilyIndex;

    VkQueue commandQueue;

    VkRenderPass renderPass;

    VkDescriptorSetLayout descriptorSetLayout;

    VkPipelineLayout pipelineLayout;

    VkPipeline pipeline;

    VkCommandPool commandPool;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;


    VkBuffer uboBuffer;
    VkDeviceMemory uboBufferMemory;

    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSet;

    VkImage renderImage;
    VkDeviceMemory renderImageMemory;
    VkImageView renderImageView;

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;

    VkFramebuffer framebuffer;

   

public:
    void initializeVulkanInstance();
    void destroyVulkanInstance();
    
    void initializePhysicalDevice();
    
    void initializeRenderDevice();
    void destroyRenderDevice();

    void initializeQueues();
    
    void initializeRenderPass();
    void destroyRenderPass();
    
    void initializeDescriptorSetLayout();
    void destroyDescriptorSetLayout();
    
    void initializePipelineLayout();
    void destroyPipelineLayout();
    
    void initializePipeline();
    void destroyPipeline();
    
    void initializeCommandPool();
    void destroyCommandPool();
    
    void initializeVertexBuffers(std::uint32_t vertexCount);
    void destroyVertexBuffers();

    void initializeUBOBuffers();
    void destroyUBOBuffers();
    
    void initializeDescriptors();
    void destroyDescriptors();
    
    void initializeRenderImage(std::uint32_t width, std::uint32_t height);
    void destroyRenderImage();
    
    void initializeTextureImage(const std::vector<std::uint8_t> &pixels, std::uint32_t width, std::uint32_t height);
    void destroyTextureImage();

    void initializeTextureSampler();
    void destroyTextureSampler();

    void updateDescriptors();

    void initializeFramebuffer(std::uint32_t width, std::uint32_t height);
    void destroyFramebuffer();




    void setVertex(const Vertex *vertices, size_t amount);
    void setUbo(const UniformBufferObject *ubo);


    void render(std::uint32_t width, std::uint32_t height, std::array<std::uint8_t, 4> backgroundColor, std::uint32_t vertexCount);
    std::vector<std::uint8_t> getImage(std::uint32_t width, std::uint32_t height);
    
private:
    VkShaderModule createShaderModule(const std::vector<std::uint8_t> &code);
    std::uint32_t findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags properties);

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    void createImage(std::uint32_t width, std::uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void copyBufferToImage(VkBuffer buffer, VkImage image, std::uint32_t width, std::uint32_t height);
    void createBuffer(std::uint32_t size, VkBufferUsageFlags usage, VkMemoryPropertyFlags property, VkBuffer &buffer, VkDeviceMemory &bufferMemory);
};
#include "graphics/renderResources.h"

#include <cstring>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics/uboCamera.h"



RenderResources::RenderResources(const RenderParametrs &parametrs, RenderDevice &device) :
    renderDevice(device)
{
    initializeModelVertices(parametrs.modelVertices, parametrs.modelVerticesCount);
    initializeUBOCamera(parametrs.cameraPitch,
        parametrs.cameraYaw,
        parametrs.cameraDistance,
        parametrs.cameraHeight,
        parametrs.cameraFOV,
        ((float)parametrs.imageWidth) / ((float)parametrs.imageHeight));
    initializeRenderImage(parametrs.imageWidth, parametrs.imageHeight);
    initializeBaseColorTexture(parametrs.baseColorTexture,
        parametrs.baseColorTextureWidth,
        parametrs.baseColorTextureHeight);
    initializeDefaultSempler();
}

RenderResources::~RenderResources() {
    destroyDefaultSampler();
    destroyBaseColorTexture();
    destroyRenderImage();
    destroyUBOCamera();
    destroyModelVertices();
}


void RenderResources::initializeModelVertices(const Vertex *vertices, unsigned int count) {
    std::size_t memSize = sizeof(Vertex) * count;

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = memSize;
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    vkCreateBuffer(renderDevice.logicalDevice, &bufferInfo, nullptr, &modelVertices);

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(renderDevice.logicalDevice, modelVertices, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    vkAllocateMemory(renderDevice.logicalDevice, &allocInfo, nullptr, &modelVerticesMemory);

    vkBindBufferMemory(renderDevice.logicalDevice, modelVertices, modelVerticesMemory, 0);

    void* data;
    vkMapMemory(renderDevice.logicalDevice, modelVerticesMemory, 0, memSize, 0, &data);
        std::memcpy(data, vertices, memSize);
    vkUnmapMemory(renderDevice.logicalDevice, modelVerticesMemory);
}

void RenderResources::destroyModelVertices() {
    vkDestroyBuffer(renderDevice.logicalDevice, modelVertices, nullptr);
    vkFreeMemory(renderDevice.logicalDevice, modelVerticesMemory, nullptr);
}


void RenderResources::initializeUBOCamera(float pitch, float yaw, float distance, float height, float fov, float aspectRatio) {
    std::size_t memSize = sizeof(UBOCamera);

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = memSize;
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    vkCreateBuffer(renderDevice.logicalDevice, &bufferInfo, nullptr, &uboCamera);

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(renderDevice.logicalDevice, uboCamera, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    vkAllocateMemory(renderDevice.logicalDevice, &allocInfo, nullptr, &uboCameraMemory);

    vkBindBufferMemory(renderDevice.logicalDevice, uboCamera, uboCameraMemory, 0);


    glm::vec3 cameraPosition(0, 0, height);

    glm::vec3 loockVector = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), glm::vec3(0.0f, 0.0f, 1.0f)) *
                            glm::rotate(glm::mat4(1.0f), glm::radians(-pitch), glm::vec3(0.0f, 1.0f, 0.0f)) *
                            glm::vec4(distance, 0.0f, 0.0f, 0.0f);

    glm::mat4 view = glm::lookAt(loockVector + cameraPosition, cameraPosition - loockVector, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 proj = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 20.0f);
    proj[1][1] *= -1;

    UBOCamera ubo{};
    ubo.model = proj * view;


    void* data;
    vkMapMemory(renderDevice.logicalDevice, uboCameraMemory, 0, memSize, 0, &data);
        std::memcpy(data, &ubo, memSize);
    vkUnmapMemory(renderDevice.logicalDevice, uboCameraMemory);
}

void RenderResources::destroyUBOCamera() {
    vkDestroyBuffer(renderDevice.logicalDevice, uboCamera, nullptr);
    vkFreeMemory(renderDevice.logicalDevice, uboCameraMemory, nullptr);
}


void RenderResources::initializeRenderImage(std::uint32_t width, std::uint32_t height) {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    vkCreateImage(renderDevice.logicalDevice, &imageInfo, nullptr, &renderImage);

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(renderDevice.logicalDevice, renderImage, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    vkAllocateMemory(renderDevice.logicalDevice, &allocInfo, nullptr, &renderImageMemory);

    vkBindImageMemory(renderDevice.logicalDevice, renderImage, renderImageMemory, 0);

    VkImageViewCreateInfo imageViewInfo{};
    imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewInfo.image = renderImage;
    imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageViewInfo.subresourceRange.baseMipLevel = 0;
    imageViewInfo.subresourceRange.levelCount = 1;
    imageViewInfo.subresourceRange.baseArrayLayer = 0;
    imageViewInfo.subresourceRange.layerCount = 1;

    vkCreateImageView(renderDevice.logicalDevice, &imageViewInfo, nullptr, &renderImageView);
}

void RenderResources::destroyRenderImage() {
    vkDestroyImageView(renderDevice.logicalDevice, renderImageView, nullptr);
    vkDestroyImage(renderDevice.logicalDevice, renderImage, nullptr);
    vkFreeMemory(renderDevice.logicalDevice, renderImageMemory, nullptr);
}


void RenderResources::initializeBaseColorTexture(const std::uint8_t *image, std::uint32_t width, std::uint32_t height) {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_LINEAR;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    vkCreateImage(renderDevice.logicalDevice, &imageInfo, nullptr, &baseColor);

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(renderDevice.logicalDevice, baseColor, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    vkAllocateMemory(renderDevice.logicalDevice, &allocInfo, nullptr, &baseColorMemory);

    vkBindImageMemory(renderDevice.logicalDevice, baseColor, baseColorMemory, 0);

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = baseColor;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(renderDevice.universalBuffer, &beginInfo);

    vkCmdPipelineBarrier(
        renderDevice.universalBuffer,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    vkEndCommandBuffer(renderDevice.universalBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &renderDevice.universalBuffer;

    vkQueueSubmit(renderDevice.universalQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(renderDevice.universalQueue);

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    std::size_t memSize = sizeof(std::uint8_t) * width * height * 4;

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = memSize;
    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    vkCreateBuffer(renderDevice.logicalDevice, &bufferInfo, nullptr, &stagingBuffer);

    vkGetBufferMemoryRequirements(renderDevice.logicalDevice, stagingBuffer, &memRequirements);

    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    vkAllocateMemory(renderDevice.logicalDevice, &allocInfo, nullptr, &stagingBufferMemory);

    vkBindBufferMemory(renderDevice.logicalDevice, stagingBuffer, stagingBufferMemory, 0);

    void* data;
    vkMapMemory(renderDevice.logicalDevice, stagingBufferMemory, 0, memSize, 0, &data);
        memcpy(data, image, memSize);
    vkUnmapMemory(renderDevice.logicalDevice, stagingBufferMemory);

    vkBeginCommandBuffer(renderDevice.universalBuffer, &beginInfo);

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
        width,
        height,
        1
    };

    vkCmdCopyBufferToImage(
        renderDevice.universalBuffer,
        stagingBuffer,
        baseColor,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );

    vkEndCommandBuffer(renderDevice.universalBuffer);

    vkQueueSubmit(renderDevice.universalQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(renderDevice.universalQueue);

    vkDestroyBuffer(renderDevice.logicalDevice, stagingBuffer, nullptr);
    vkFreeMemory(renderDevice.logicalDevice, stagingBufferMemory, nullptr);

    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkBeginCommandBuffer(renderDevice.universalBuffer, &beginInfo);

    vkCmdPipelineBarrier(
        renderDevice.universalBuffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    vkEndCommandBuffer(renderDevice.universalBuffer);

    vkQueueSubmit(renderDevice.universalQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(renderDevice.universalQueue);

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = baseColor;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    vkCreateImageView(renderDevice.logicalDevice, &viewInfo, nullptr, &baseColorView);
}

void RenderResources::destroyBaseColorTexture() {
    vkDestroyImageView(renderDevice.logicalDevice, baseColorView, nullptr);
    vkFreeMemory(renderDevice.logicalDevice, baseColorMemory, nullptr);
    vkDestroyImage(renderDevice.logicalDevice, baseColor, nullptr);
}


void RenderResources::initializeDefaultSempler() {
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    vkCreateSampler(renderDevice.logicalDevice, &samplerInfo, nullptr, &defaultSampler);
}

void RenderResources::destroyDefaultSampler() {
    vkDestroySampler(renderDevice.logicalDevice, defaultSampler, nullptr);
}



std::uint32_t RenderResources::findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags properties) const {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(renderDevice.physicalDevice, &memProperties);

    for (std::uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    return 0;
}
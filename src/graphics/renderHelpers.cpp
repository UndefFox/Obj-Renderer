#include "graphics/renderHelpers.h"

#include "fileController.h"

#include <cstring>
#include <array>

RenderHelpers::RenderHelpers(RenderDevice &device) :
    renderDevice(device)
{

}

RenderHelpers::~RenderHelpers() {

}


std::uint32_t RenderHelpers::findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags properties) const {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(renderDevice.physicalDevice, &memProperties);

    for (std::uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    return 0;
}

VkShaderModule RenderHelpers::createShaderModule(const std::string filePath) {
    VkShaderModule output;

    std::vector<std::uint8_t> code = FileController::readBinaryFile(filePath);

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const std::uint32_t*>(code.data());

    vkCreateShaderModule(renderDevice.logicalDevice, &createInfo, nullptr, &output);

    return output;
}

void RenderHelpers::transitImageLayout(VkImage &image, VkImageLayout oldLayout, VkImageLayout newLayout, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask) {
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = srcAccessMask;
    barrier.dstAccessMask = dstAccessMask;

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(renderDevice.universalBuffer, &beginInfo);

    vkCmdPipelineBarrier(
        renderDevice.universalBuffer,
        srcStageMask, dstStageMask,
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
}

void RenderHelpers::writeToImage(VkImage &image, VkImageLayout imageLayout, const std::uint8_t *imageData, unsigned int width, unsigned int height) {
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    std::size_t memSize = sizeof(std::uint8_t) * width * height * 4;

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = memSize;
    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    vkCreateBuffer(renderDevice.logicalDevice, &bufferInfo, nullptr, &stagingBuffer);

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(renderDevice.logicalDevice, stagingBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    vkAllocateMemory(renderDevice.logicalDevice, &allocInfo, nullptr, &stagingBufferMemory);

    vkBindBufferMemory(renderDevice.logicalDevice, stagingBuffer, stagingBufferMemory, 0);

    void* data;
    vkMapMemory(renderDevice.logicalDevice, stagingBufferMemory, 0, memSize, 0, &data);
        memcpy(data, imageData, memSize);
    vkUnmapMemory(renderDevice.logicalDevice, stagingBufferMemory);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

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
        image,
        imageLayout,
        1,
        &region
    );

    vkEndCommandBuffer(renderDevice.universalBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &renderDevice.universalBuffer;

    vkQueueSubmit(renderDevice.universalQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(renderDevice.universalQueue);

    vkFreeMemory(renderDevice.logicalDevice, stagingBufferMemory, nullptr);
    vkDestroyBuffer(renderDevice.logicalDevice, stagingBuffer, nullptr);
}

void RenderHelpers::getImage(VkImage &image, VkImageLayout imageLayout, std::uint8_t *imageData, unsigned int width, unsigned int height) {
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    std::size_t memSize = sizeof(std::uint8_t) * width * height * 4;

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = memSize;
    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    vkCreateBuffer(renderDevice.logicalDevice, &bufferInfo, nullptr, &stagingBuffer);

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(renderDevice.logicalDevice, image, &memRequirements);

    vkGetBufferMemoryRequirements(renderDevice.logicalDevice, stagingBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    vkAllocateMemory(renderDevice.logicalDevice, &allocInfo, nullptr, &stagingBufferMemory);

    vkBindBufferMemory(renderDevice.logicalDevice, stagingBuffer, stagingBufferMemory, 0);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

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

    vkCmdCopyImageToBuffer(
        renderDevice.universalBuffer,
        image,
        VK_IMAGE_LAYOUT_GENERAL,
        stagingBuffer,
        1,
        &region
    );

    vkEndCommandBuffer(renderDevice.universalBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &renderDevice.universalBuffer;

    vkQueueSubmit(renderDevice.universalQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(renderDevice.universalQueue);

    void *data;
    vkMapMemory(renderDevice.logicalDevice, stagingBufferMemory, 0, memSize, 0, &data);
        std::memcpy(imageData, data, memSize);
    vkUnmapMemory(renderDevice.logicalDevice, stagingBufferMemory);

    vkDestroyBuffer(renderDevice.logicalDevice, stagingBuffer, nullptr);
    vkFreeMemory(renderDevice.logicalDevice, stagingBufferMemory, nullptr);
}


std::vector<Vertex> RenderHelpers::getTextGeometry(std::string text, glm::vec3 topCorner, glm::vec3 rightDirection, glm::vec3 downDirection, float scale, bool mirror, bool bottomAligment) {
    std::vector<Vertex> output;

    rightDirection = glm::normalize(rightDirection) * scale;
    downDirection = glm::normalize(downDirection) * scale;

    if (mirror) {
        text = std::string(text.rbegin(), text.rend());
    }

    std::array<Vertex, 6> vertices;

    if (mirror && !bottomAligment) {
        vertices = {
            Vertex{.pos = downDirection                 , .texCoord = glm::vec2{0.25f, 0.25f}},  //left bottom
            Vertex{.pos = rightDirection                , .texCoord = glm::vec2{0.00f, 0.00f}},  //right top
            Vertex{.pos = rightDirection + downDirection, .texCoord = glm::vec2{0.00f, 0.25f}},  //right bottom
            Vertex{.pos = downDirection                 , .texCoord = glm::vec2{0.25f, 0.25f}},  //left bottom
            Vertex{.pos = {0, 0, 0}                     , .texCoord = glm::vec2{0.25f, 0.00f}},  //left top
            Vertex{.pos = rightDirection                , .texCoord = glm::vec2{0.00f, 0.00f}}   //right top
        };
    }
    else if (!mirror && !bottomAligment) {
        vertices = {
            Vertex{.pos = rightDirection + downDirection, .texCoord = glm::vec2{0.25f, 0.25f}},  //right bottom
            Vertex{.pos = rightDirection                , .texCoord = glm::vec2{0.25f, 0.00f}},  //right top
            Vertex{.pos = downDirection                 , .texCoord = glm::vec2{0.00f, 0.25f}},  //left bottom
            Vertex{.pos = rightDirection                , .texCoord = glm::vec2{0.25f, 0.00f}},  //right top
            Vertex{.pos = {0, 0, 0}                     , .texCoord = glm::vec2{0.00f, 0.00f}},  //left top
            Vertex{.pos = downDirection                 , .texCoord = glm::vec2{0.00f, 0.25f}}   //left bottom
        };
    }
    else if (mirror && bottomAligment) {
        vertices = {
            Vertex{.pos = {0, 0, 0}                     , .texCoord = glm::vec2{0.25f, 0.25f}},  //left bottom
            Vertex{.pos = rightDirection - downDirection, .texCoord = glm::vec2{0.00f, 0.00f}},  //right top
            Vertex{.pos = rightDirection                , .texCoord = glm::vec2{0.00f, 0.25f}},  //right bottom
            Vertex{.pos = {0, 0, 0}                     , .texCoord = glm::vec2{0.25f, 0.25f}},  //left bottom
            Vertex{.pos = -downDirection                , .texCoord = glm::vec2{0.25f, 0.00f}},  //left top
            Vertex{.pos = rightDirection - downDirection, .texCoord = glm::vec2{0.00f, 0.00f}}   //right top
        };
    }
    else if (!mirror && bottomAligment) {
        vertices = {
            Vertex{.pos = rightDirection                , .texCoord = glm::vec2{0.25f, 0.25f}},  //right bottom
            Vertex{.pos = rightDirection - downDirection, .texCoord = glm::vec2{0.25f, 0.00f}},  //right top
            Vertex{.pos = {0, 0, 0}                     , .texCoord = glm::vec2{0.00f, 0.25f}},  //left bottom
            Vertex{.pos = rightDirection - downDirection, .texCoord = glm::vec2{0.25f, 0.00f}},  //right top
            Vertex{.pos = -downDirection                , .texCoord = glm::vec2{0.00f, 0.00f}},  //left top
            Vertex{.pos = {0, 0, 0}                     , .texCoord = glm::vec2{0.00f, 0.25f}}   //left bottom
        };
    }

    for (char sym : text) {
        glm::vec2 texOffset;

        switch (sym) {
            case '0': texOffset = {0.00f, 0.00f}; break;
            case '1': texOffset = {0.25f, 0.00f}; break;
            case '2': texOffset = {0.50f, 0.00f}; break;
            case '3': texOffset = {0.75f, 0.00f}; break;
            case '4': texOffset = {0.00f, 0.25f}; break;
            case '5': texOffset = {0.25f, 0.25f}; break;
            case '6': texOffset = {0.50f, 0.25f}; break;
            case '7': texOffset = {0.75f, 0.25f}; break;
            case '8': texOffset = {0.00f, 0.50f}; break;
            case '9': texOffset = {0.25f, 0.50f}; break;
            case '.': texOffset = {0.50f, 0.50f}; break;
            case 'm': texOffset = {0.75f, 0.50f}; break;
        }

        for (Vertex vert : vertices) {
            output.push_back(Vertex{.pos = vert.pos + topCorner, .texCoord = vert.texCoord + texOffset});
        }

        topCorner += rightDirection;
    }

    return output;
}
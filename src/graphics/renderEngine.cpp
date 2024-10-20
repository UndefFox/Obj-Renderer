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
    std::size_t count = parametrs.imageWidth * parametrs.imageHeight * 4;

    std::vector<std::uint8_t> pixels(count);
    void *data;
    vkMapMemory(device.logicalDevice, resources.renderImageMemory, 0, VK_WHOLE_SIZE, 0, &data);
        std::memcpy(pixels.data(), data, sizeof(std::uint8_t) * count);
    vkUnmapMemory(device.logicalDevice, resources.renderImageMemory);

    return pixels;
}
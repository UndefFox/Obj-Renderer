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
    helpers(device),
    resources(parametrs, device, helpers),
    colorPass(parametrs, device, resources, helpers)
{}

RenderEngine::~RenderEngine() { }


void RenderEngine::render() {
    colorPass.render(parametrs);
}

std::vector<std::uint8_t> RenderEngine::getImage() {
    helpers.transitImageLayout(resources.renderImage,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL,
        0, 0,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
    );
    
    std::vector<std::uint8_t> pixels(parametrs.imageWidth * parametrs.imageHeight * 4);
    helpers.getImage(resources.renderImage, 
        VK_IMAGE_LAYOUT_GENERAL,
        pixels.data(),
        parametrs.imageWidth, parametrs.imageHeight
    );

    return pixels;
}
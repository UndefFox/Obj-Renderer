#include "./renderEngine.h"

#include "./privateRenderEngine.h"

#include <array>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "./vertex.h"
#include "./ubo.h"

RenderEngine::RenderEngine() : 
    privateRenderEngine(new PrivateRenderEngine),
    renderWidht(128),
    renderHeight(128),
    renderBackgroundColor({0, 0, 0, 0}),
    vertexCount(0)
{}

RenderEngine::~RenderEngine() {
    delete(privateRenderEngine);
}

void RenderEngine::initialize() {
    privateRenderEngine->initializeVulkanInstance();
    privateRenderEngine->initializePhysicalDevice();
    privateRenderEngine->initializeRenderDevice();
    privateRenderEngine->initializeQueues();
    privateRenderEngine->initializeRenderPass();
    privateRenderEngine->initializeDescriptorSetLayout();
    privateRenderEngine->initializePipelineLayout();
    privateRenderEngine->initializePipeline();
    privateRenderEngine->initializeCommandPool();
    privateRenderEngine->initializeVertexBuffers(1);
    privateRenderEngine->initializeUBOBuffers();
    privateRenderEngine->initializeDescriptors();
    privateRenderEngine->initializeRenderImage(renderWidht, renderHeight);
    privateRenderEngine->initializeTextureImage(std::vector<std::uint8_t>{128, 255, 1, 255}, 1, 1);
    privateRenderEngine->initializeTextureSampler();
    privateRenderEngine->updateDescriptors();
    privateRenderEngine->initializeFramebuffer(renderWidht, renderHeight);
    
}

void RenderEngine::destroy() {
    privateRenderEngine->destroyTextureSampler();
    privateRenderEngine->destroyTextureImage();
    privateRenderEngine->destroyFramebuffer();
    privateRenderEngine->destroyRenderImage();
    privateRenderEngine->destroyDescriptors();
    privateRenderEngine->destroyUBOBuffers();
    privateRenderEngine->destroyVertexBuffers();
    privateRenderEngine->destroyCommandPool();
    privateRenderEngine->destroyPipeline();
    privateRenderEngine->destroyPipelineLayout();
    privateRenderEngine->destroyDescriptorSetLayout();
    privateRenderEngine->destroyRenderPass();
    privateRenderEngine->destroyRenderDevice();
    privateRenderEngine->destroyVulkanInstance();
}

void RenderEngine::setVetecies(const std::vector<Vertex> &vertices) {
    privateRenderEngine->destroyVertexBuffers();

    vertexCount = vertices.size();

    privateRenderEngine->initializeVertexBuffers(vertexCount);
    privateRenderEngine->setVertex(vertices.data(), vertices.size());
}

void RenderEngine::setCamera(float pitch, float yaw, float distance, float height, float fov) {
    glm::vec3 cameraPosition(0, 0, height);

    glm::vec3 loockVector = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), glm::vec3(0.0f, 0.0f, 1.0f)) *
                            glm::rotate(glm::mat4(1.0f), glm::radians(-pitch), glm::vec3(0.0f, 1.0f, 0.0f)) *
                            glm::vec4(distance, 0.0f, 0.0f, 0.0f);

    glm::mat4 view = glm::lookAt(loockVector + cameraPosition, cameraPosition - loockVector, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 proj = glm::perspective(glm::radians(fov), 1.0f, 0.1f, 20.0f);
    proj[1][1] *= -1;

    UniformBufferObject ubo{};
    ubo.model = proj * view;

    privateRenderEngine->setUbo(&ubo);
}

void RenderEngine::setImage(std::uint32_t imageSize, std::array<std::uint8_t, 4> backgroundColor) {
    privateRenderEngine->destroyFramebuffer();
    privateRenderEngine->destroyRenderImage();

    renderWidht = imageSize;
    renderHeight = imageSize;
    renderBackgroundColor = backgroundColor;

    privateRenderEngine->initializeRenderImage(renderWidht, renderHeight);
    privateRenderEngine->initializeFramebuffer(renderWidht, renderHeight);
}

void RenderEngine::setTexture(std::vector<std::uint8_t> pixels, std::uint32_t width, std::uint32_t height) {
    privateRenderEngine->destroyTextureSampler();
    privateRenderEngine->destroyTextureImage();

    privateRenderEngine->initializeTextureImage(pixels, width, height);
    privateRenderEngine->initializeTextureSampler();

    privateRenderEngine->updateDescriptors();
}


void RenderEngine::render() {
    privateRenderEngine->render(renderWidht, renderHeight, renderBackgroundColor, vertexCount);
}

std::vector<std::uint8_t> RenderEngine::getImage() {
    return privateRenderEngine->getImage(renderWidht, renderWidht);
}
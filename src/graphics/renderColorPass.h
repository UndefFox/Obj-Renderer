#pragma once

#include "graphics/renderDevice.h"
#include "graphics/renderResources.h"
#include "graphics/renderParametrs.h"

#include <vector>

class RenderColorPass {
public:

private:
    VkRenderPass renderPass;

    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSet;

    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;

    VkFramebuffer framebuffer;


    RenderDevice &renderDevice;
    RenderResources &renderResources;

public:
    RenderColorPass() = delete;
    RenderColorPass(const RenderParametrs &parametrs, RenderDevice &device, RenderResources &resources);
    ~RenderColorPass();
    
    void initializeRenderPass();
    void destroyRenderPass();

    void initializeDescriptorSetLayout();
    void destroyDescriptorSetLayout();

    void initializeDescriptorPool();
    void destroyDescriptorPool();

    void initializeDescriptorSet();

    void initializatePipeline();
    void destroyPipeline();

    void initializeFramebuffer(std::uint32_t width, std::uint32_t height);
    void destroyFramebuffer();

    VkShaderModule createShaderModule(const std::vector<std::uint8_t> &code);


    void render(const RenderParametrs &parametrs);
};
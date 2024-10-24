#pragma once

#include "graphics/renderDevice.h"
#include "graphics/renderResources.h"
#include "graphics/renderParametrs.h"
#include "graphics/renderHelpers.h"

#include <vector>

class RenderTextPass {
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
    RenderHelpers &renderHelpers;

public:
    RenderTextPass() = delete;
    RenderTextPass(const RenderParametrs &parametrs, RenderDevice &device, RenderResources &resources, RenderHelpers &helpers);
    ~RenderTextPass();
    
    void initializeRenderPass();
    void destroyRenderPass();

    void initializeDescriptorSetLayout();
    void destroyDescriptorSetLayout();

    void initializeDescriptorPool();
    void destroyDescriptorPool();

    void initializeDescriptorSet();

    void initializatePipeline(std::uint32_t width, std::uint32_t height);
    void destroyPipeline();

    void initializeFramebuffer(std::uint32_t width, std::uint32_t height);
    void destroyFramebuffer();


    void render();
};
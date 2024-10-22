#pragma once

#include <vector>

#include "graphics/renderDevice.h"
#include "graphics/renderHelpers.h"
#include "graphics/renderResources.h"
#include "graphics/renderColorPass.h"

#include "graphics/renderParametrs.h"

class RenderEngine {
private:
    RenderParametrs parametrs;
    RenderDevice device;
    RenderHelpers helpers;
    RenderResources resources;
    RenderColorPass colorPass;

public:
    RenderEngine(const RenderParametrs &params);
    ~RenderEngine();

    void render();
    std::vector<std::uint8_t> getImage();
};
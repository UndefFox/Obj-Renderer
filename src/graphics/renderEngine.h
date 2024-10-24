#pragma once

#include <vector>

#include "graphics/renderDevice.h"
#include "graphics/renderHelpers.h"
#include "graphics/renderResources.h"
#include "graphics/renderColorPass.h"
#include "graphics/renderLinePass.h"
#include "graphics/renderTextPass.h"

#include "graphics/renderParametrs.h"

class RenderEngine {
private:
    RenderDevice device;
    RenderHelpers helpers;
    RenderResources resources;
    

    RenderColorPass colorPass;
    RenderLinePass linePass;
    RenderTextPass textPass;

public:
    RenderEngine(const RenderParametrs &params);
    ~RenderEngine();

    void render();
    std::vector<std::uint8_t> getImage();
};
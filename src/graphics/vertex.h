#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <array>

struct Vertex {
    alignas(16) glm::vec3 pos;
    alignas(16) glm::vec2 texCoord;
};
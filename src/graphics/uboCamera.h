#pragma once

#include <glm/glm.hpp>
    
struct UBOCamera {
    alignas(16) glm::mat4 model;
};
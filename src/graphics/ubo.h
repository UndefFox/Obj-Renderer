#ifndef H_RENDERENGINE_UBO
#define H_RENDERENGINE_UBO

#include <glm/glm.hpp>
    
struct UniformBufferObject {
    alignas(16) glm::mat4 model;
};

#endif
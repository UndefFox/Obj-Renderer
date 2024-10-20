#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCord;

layout(location = 0) out vec2 fragTextCord;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
} ubo;

void main() {
    gl_Position = ubo.model * vec4(inPosition, 1.0);

    fragTextCord = inTexCord;
}
#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 modelMatrix;
    mat4 viewMatrix;
    mat4 projectionMatrix;
} ubo;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 textureCoordinates;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTextureCoordinates;

void main() {
    gl_Position = ubo.projectionMatrix * ubo.viewMatrix * ubo.modelMatrix * vec4(position, 1.0);
    fragColor = color;
    fragTextureCoordinates = textureCoordinates;
}

#version 450

layout(set = 0, binding = 0) uniform ShadowMapRendererVertexUniformBufferObject {
    mat4 viewMatrix;
    mat4 projectionMatrix;
} smrvubo;

layout(set = 1, binding = 0) uniform EntityVertexUniformBufferObject {
    mat4 modelMatrix;
} evubo;

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 textureCoordinates;

layout(location = 0) out vec2 fragTextureCoordinates;

void main() {
    gl_Position = smrvubo.projectionMatrix * smrvubo.viewMatrix * evubo.modelMatrix * vec4(position, 1.0);
    fragTextureCoordinates = textureCoordinates;
}

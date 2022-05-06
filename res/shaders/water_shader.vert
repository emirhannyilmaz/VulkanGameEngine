#version 450

vec3 vertices[6] = vec3[] (
    vec3(-1.0, 0.0, -1.0),
    vec3(-1.0, 0.0, 1.0),
    vec3(1.0, 0.0, 1.0),
    vec3(-1.0, 0.0, -1.0),
    vec3(1.0, 0.0, 1.0),
    vec3(1.0, 0.0, -1.0)
);

layout(set = 0, binding = 0) uniform WaterRendererVertexUniformBufferObject {
    mat4 viewMatrix;
    mat4 projectionMatrix;
} wrvubo;

layout(set = 1, binding = 0) uniform WaterTileVertexUniformBufferObject {
    mat4 modelMatrix;
} wtvubo;

layout(location = 0) out vec4 clipSpace;
layout(location = 1) out vec2 textureCoordinates;

const float tiling = 6.0;

void main() {
    vec3 position = vertices[gl_VertexIndex];
    clipSpace = wrvubo.projectionMatrix * wrvubo.viewMatrix * wtvubo.modelMatrix * vec4(position, 1.0);
    gl_Position = clipSpace;
    textureCoordinates = vec2(position.x / 2.0 + 0.5, position.z / 2.0 + 0.5) * tiling;
}

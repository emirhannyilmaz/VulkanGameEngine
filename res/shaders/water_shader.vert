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

layout(location = 0) out vec4 fragClipSpace;
layout(location = 1) out vec2 fragTextureCoordinates;
layout(location = 2) out vec3 fragToCameraVector;

const float tiling = 6.0;

void main() {
    vec3 vertexPosition = vertices[gl_VertexIndex];
    vec4 worldPosition = wtvubo.modelMatrix * vec4(vertexPosition, 1.0);
    fragClipSpace = wrvubo.projectionMatrix * wrvubo.viewMatrix * worldPosition;
    gl_Position = fragClipSpace;
    fragTextureCoordinates = vec2(vertexPosition.x / 2.0 + 0.5, vertexPosition.z / 2.0 + 0.5) * tiling;
    fragToCameraVector = (inverse(wrvubo.viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
}

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

void main() {
    gl_Position = wrvubo.projectionMatrix * wrvubo.viewMatrix * wtvubo.modelMatrix * vec4(vertices[gl_VertexIndex], 1.0);
}

#version 450

vec3 vertices[36] = vec3[] (
    vec3(-1.0, 1.0, 1.0),
    vec3(-1.0, -1.0, 1.0),
    vec3(1.0, -1.0, 1.0),
    vec3(1.0, -1.0, 1.0),
    vec3(1.0, 1.0, 1.0),
    vec3(-1.0, 1.0, 1.0),

    vec3(-1.0, -1.0, -1.0),
    vec3(-1.0, -1.0, 1.0),
    vec3(-1.0, 1.0, 1.0),
    vec3(-1.0, 1.0, 1.0),
    vec3(-1.0, 1.0, -1.0),
    vec3(-1.0, -1.0, -1.0),

    vec3(1.0, -1.0, 1.0),
    vec3(1.0, -1.0, -1.0),
    vec3(1.0, 1.0, -1.0),
    vec3(1.0, 1.0, -1.0),
    vec3(1.0, 1.0, 1.0),
    vec3(1.0, -1.0, 1.0),

    vec3(-1.0, -1.0, -1.0),
    vec3(-1.0, 1.0, -1.0),
    vec3(1.0, 1.0, -1.0),
    vec3(1.0, 1.0, -1.0),
    vec3(1.0, -1.0, -1.0),
    vec3(-1.0, -1.0, -1.0),

    vec3(-1.0, 1.0, 1.0),
    vec3(1.0, 1.0, 1.0),
    vec3(1.0, 1.0, -1.0),
    vec3(1.0, 1.0, -1.0),
    vec3(-1.0, 1.0, -1.0),
    vec3(-1.0, 1.0, 1.0),

    vec3(-1.0, -1.0, 1.0),
    vec3(-1.0, -1.0, -1.0),
    vec3(1.0, -1.0, 1.0),
    vec3(1.0, -1.0, 1.0),
    vec3(-1.0, -1.0, -1.0),
    vec3(1.0, -1.0, -1.0)
);

layout(set = 0, binding = 0) uniform SkyboxRendererVertexUniformBufferObject {
    mat4 projectionMatrix;
} srvubo;

layout(set = 1, binding = 0) uniform SkyboxVertexUniformBufferObject {
    float size;
} svubo;

layout(push_constant) uniform SkyboxRendererVertexPushConstants {
    mat4 viewMatrix;
    vec4 clipPlane;
} srvpc;

layout(location = 0) out vec3 fragTextureCoordinates;

void main() {
    vec4 worldPosition = vec4(vertices[gl_VertexIndex] * svubo.size, 1.0);

    gl_ClipDistance[0] = dot(worldPosition, srvpc.clipPlane);
    gl_Position = srvubo.projectionMatrix * srvpc.viewMatrix * worldPosition;

    fragTextureCoordinates = worldPosition.xyz * vec3(1.0, -1.0, 1.0);
}

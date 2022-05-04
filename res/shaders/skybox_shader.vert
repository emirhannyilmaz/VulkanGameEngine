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
    mat4 viewMatrix;
    mat4 projectionMatrix;
    vec4 clipPlane;
} srvubo;

layout(set = 1, binding = 0) uniform SkyboxVertexUniformBufferObject {
    float size;
} svubo;

layout(location = 0) out vec3 fragTextureCoordinates;

void main() {
    vec4 worldPosition = vec4(vertices[gl_VertexIndex] * svubo.size, 1.0);

    gl_ClipDistance[0] = dot(worldPosition, srvubo.clipPlane);
    gl_Position = srvubo.projectionMatrix * srvubo.viewMatrix * worldPosition;

    vec3 textureCoordinates = vertices[gl_VertexIndex] * vec3(1.0, -1.0, 1.0);
    fragTextureCoordinates = textureCoordinates;
}

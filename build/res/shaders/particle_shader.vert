#version 450

vec3 vertices[6] = vec3[] (
    vec3(-1.0, 1.0, 0.0),
    vec3(-1.0, -1.0, 0.0),
    vec3(1.0, -1.0, 0.0),
    vec3(1.0, -1.0, 0.0),
    vec3(1.0, 1.0, 0.0),
    vec3(-1.0, 1.0, 0.0)
);

layout(set = 0, binding = 0) uniform ParticleRendererVertexUniformBufferObject {
    mat4 projectionMatrix;
} prvubo;

layout(set = 1, binding = 0) uniform ParticleVertexUniformBufferObject {
    mat4 modelMatrix;
    vec2 textureOffset1;
    vec2 textureOffset2;
    int rowCount;
} pvubo;

layout(push_constant) uniform VertexPushConstants {
    vec4 clipPlane;
    mat4 modelViewMatrix;
} vpc;

layout(location = 0) out float fragVisibility;
layout(location = 1) out vec2 fragTextureCoordinates1;
layout(location = 2) out vec2 fragTextureCoordinates2;

const float fogDensity = 0.004;
const float fogGradient = 1.5;

vec2 convertVertexCoordinatesToTextureCoordinates (vec3 vertexCoordinates) {
    float oldMax = 1.0;
    float oldMin = -1.0;
    float newMax = 1.0;
    float newMin = 0.0;
    float oldRange = (oldMax - oldMin);
    float newRange = (newMax - newMin);
    return vec2((((vertexCoordinates.x - oldMin) * newRange) / oldRange) + newMin, (((vertexCoordinates.y - oldMin) * newRange) / oldRange) + newMin);
}

void main() {
    vec2 textureCoordinates = convertVertexCoordinatesToTextureCoordinates(vertices[gl_VertexIndex]);
    textureCoordinates /= pvubo.rowCount;
    fragTextureCoordinates1 = textureCoordinates + pvubo.textureOffset1;
    fragTextureCoordinates2 = textureCoordinates + pvubo.textureOffset2;

    gl_ClipDistance[0] = dot(pvubo.modelMatrix * vec4(vertices[gl_VertexIndex], 1.0), vpc.clipPlane);
    vec4 positionRelativeToCamera = vpc.modelViewMatrix * vec4(vertices[gl_VertexIndex], 1.0);
    gl_Position = prvubo.projectionMatrix * positionRelativeToCamera;

    float distance = length(positionRelativeToCamera.xyz);
    fragVisibility = exp(-pow(distance * fogDensity, fogGradient));
    fragVisibility = clamp(fragVisibility, 0.0, 1.0);
}

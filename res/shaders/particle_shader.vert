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
} pvubo;

layout(push_constant) uniform ParticleRendererVertexPushConstants {
    mat4 viewMatrix;
    vec4 clipPlane;
} prvpc;

layout(location = 0) out float fragVisibility;

const float fogDensity = 0.004;
const float fogGradient = 1.5;

void main() {
    gl_ClipDistance[0] = dot(pvubo.modelMatrix * vec4(vertices[gl_VertexIndex], 1.0), prvpc.clipPlane);
    mat4 modelViewMatrix = mat4(1.0);
    modelViewMatrix[3] = mat4(prvpc.viewMatrix * pvubo.modelMatrix)[3];
    vec4 positionRelativeToCamera = modelViewMatrix * vec4(vertices[gl_VertexIndex], 1.0);
    gl_Position = prvubo.projectionMatrix * positionRelativeToCamera;

    float distance = length(positionRelativeToCamera.xyz);
    fragVisibility = exp(-pow(distance * fogDensity, fogGradient));
    fragVisibility = clamp(fragVisibility, 0.0, 1.0);
}

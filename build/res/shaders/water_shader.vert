#version 450

layout(set = 0, binding = 0) uniform WaterRendererVertexUniformBufferObject {
    mat4 viewMatrix;
    mat4 projectionMatrix;
    vec3 lightPosition;
} wrvubo;

layout(set = 1, binding = 0) uniform WaterTileVertexUniformBufferObject {
    mat4 modelMatrix;
} wtvubo;

layout(location = 0) in vec3 position;

layout(location = 0) out vec4 fragClipSpace;
layout(location = 1) out vec2 fragTextureCoordinates;
layout(location = 2) out vec3 fragToCameraVector;
layout(location = 3) out vec3 fragFromLightVector;
layout(location = 4) out float fragVisibility;

const float tiling = 20.0;
const float fogDensity = 0.004;
const float fogGradient = 1.5;

void main() {
    vec4 worldPosition = wtvubo.modelMatrix * vec4(position, 1.0);
    vec4 positionRelativeToCamera = wrvubo.viewMatrix * worldPosition;
    fragClipSpace = wrvubo.projectionMatrix * positionRelativeToCamera;
    gl_Position = fragClipSpace;
    fragTextureCoordinates = vec2(position.x / 2.0 + 0.5, position.z / 2.0 + 0.5) * tiling;
    fragToCameraVector = (inverse(wrvubo.viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
    fragFromLightVector = worldPosition.xyz - wrvubo.lightPosition;

    float distance = length(positionRelativeToCamera.xyz);
    fragVisibility = exp(-pow(distance * fogDensity, fogGradient));
    fragVisibility = clamp(fragVisibility, 0.0, 1.0);
}

#version 450

layout(set = 0, binding = 0) uniform TerrainRendererVertexUniformBufferObject {
    mat4 projectionMatrix;
    vec3 lightPosition;
    mat4 toShadowMapSpaceMatrix;
    float shadowDistance;
} trvubo;

layout(set = 1, binding = 0) uniform TerrainVertexUniformBufferObject {
    mat4 modelMatrix;
} tvubo;

layout(push_constant) uniform TerrainRendererVertexPushConstants {
    mat4 viewMatrix;
    vec4 clipPlane;
} trvpc;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoordinates;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec2 fragTextureCoordinates;
layout(location = 2) out vec3 fragToLightVector;
layout(location = 3) out vec3 fragToCameraVector;
layout(location = 4) out vec4 fragPositionInShadowMap;
layout(location = 5) out float fragVisibility;

const float tiling = 40.0;
const float transitionDistance = 10.0;
const float fogDensity = 0.004;
const float fogGradient = 1.5;

void main() {
    vec4 worldPosition = tvubo.modelMatrix * vec4(position, 1.0);
    fragPositionInShadowMap = trvubo.toShadowMapSpaceMatrix * worldPosition;
    gl_ClipDistance[0] = dot(worldPosition, trvpc.clipPlane);
    vec4 positionRelativeToCamera = trvpc.viewMatrix * worldPosition;
    gl_Position = trvubo.projectionMatrix * positionRelativeToCamera;
    fragNormal = (tvubo.modelMatrix * vec4(normal, 0.0)).xyz;
    fragTextureCoordinates = textureCoordinates * tiling;
    fragToLightVector = trvubo.lightPosition - worldPosition.xyz;
    fragToCameraVector = (inverse(trvpc.viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;

    float distance = length(positionRelativeToCamera.xyz);
    fragVisibility = exp(-pow(distance * fogDensity, fogGradient));
    fragVisibility = clamp(fragVisibility, 0.0, 1.0);

    distance = distance - (trvubo.shadowDistance - transitionDistance);
    distance = distance / transitionDistance;
    fragPositionInShadowMap.w = clamp(1.0 - distance, 0.0, 1.0);
}

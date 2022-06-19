#version 450

const int maxJoints = 50;
const int maxWeights = 3;

layout(set = 0, binding = 0) uniform AnimatedEntityRendererVertexUniformBufferObject {
    mat4 projectionMatrix;
    vec3 lightPosition;
} aervubo;

layout(set = 1, binding = 0) uniform AnimatedEntityVertexUniformBufferObject {
    mat4 modelMatrix;
    mat4 jointTransforms[maxJoints];
} aevubo;

layout(push_constant) uniform AnimatedEntityRendererVertexPushConstants {
    mat4 viewMatrix;
    vec4 clipPlane;
} aervpc;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoordinates;
layout(location = 3) in ivec3 jointIDs;
layout(location = 4) in vec3 jointWeights;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec2 fragTextureCoordinates;
layout(location = 2) out vec3 fragToLightVector;
layout(location = 3) out vec3 fragToCameraVector;
layout(location = 4) out float fragVisibility;

const float fogDensity = 0.01;
const float fogGradient = 1.2;

void main() {
    vec4 totalPosition = vec4(0.0);
    vec4 totalNormal = vec4(0.0);

    for (int i = 0; i < maxWeights; i++) {
        mat4 jointTransform = aevubo.jointTransforms[jointIDs[i]];

        vec4 tPosition = jointTransform * vec4(position, 1.0);
        totalPosition += tPosition * jointWeights[i];

        vec4 tNormal = jointTransform * vec4(normal, 0.0);
        totalNormal += tNormal * jointWeights[i];
    }

    vec4 worldPosition = aevubo.modelMatrix * totalPosition;
    gl_ClipDistance[0] = dot(worldPosition, aervpc.clipPlane);
    vec4 positionRelativeToCamera = aervpc.viewMatrix * worldPosition;
    gl_Position = aervubo.projectionMatrix * positionRelativeToCamera;
    fragNormal = (aevubo.modelMatrix * totalNormal).xyz;
    fragTextureCoordinates = textureCoordinates;
    fragToLightVector = aervubo.lightPosition - worldPosition.xyz;
    fragToCameraVector = (inverse(aervpc.viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;

    float distance = length(positionRelativeToCamera.xyz);
    fragVisibility = exp(-pow(distance * fogDensity, fogGradient));
    fragVisibility = clamp(fragVisibility, 0.0, 1.0);
}

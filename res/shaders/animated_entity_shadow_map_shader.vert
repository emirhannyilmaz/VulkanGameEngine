#version 450

const int maxJoints = 50;
const int maxWeights = 3;

layout(set = 0, binding = 0) uniform AnimatedEntityShadowMapRendererVertexUniformBufferObject {
    mat4 viewMatrix;
    mat4 projectionMatrix;
} aesmrvubo;

layout(set = 1, binding = 0) uniform AnimatedEntityVertexUniformBufferObject {
    mat4 modelMatrix;
    mat4 jointTransforms[maxJoints];
} aevubo;

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 textureCoordinates;
layout(location = 3) in ivec3 jointIDs;
layout(location = 4) in vec3 jointWeights;

layout(location = 0) out vec2 fragTextureCoordinates;

void main() {
    vec4 totalPosition = vec4(0.0);

    for (int i = 0; i < maxWeights; i++) {
        mat4 jointTransform = aevubo.jointTransforms[jointIDs[i]];

        vec4 tPosition = jointTransform * vec4(position, 1.0);
        totalPosition += tPosition * jointWeights[i];
    }

    gl_Position = aesmrvubo.projectionMatrix * aesmrvubo.viewMatrix * aevubo.modelMatrix * totalPosition;
    fragTextureCoordinates = textureCoordinates;
}

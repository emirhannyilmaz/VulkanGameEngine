#version 450

layout(set = 0, binding = 0) uniform EntityRendererVertexUniformBufferObject {
    mat4 viewMatrix;
    mat4 projectionMatrix;
    vec3 lightPosition;
} ervubo;

layout(set = 1, binding = 0) uniform EntityVertexUniformBufferObject {
    mat4 modelMatrix;
} evubo;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoordinates;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec2 fragTextureCoordinates;
layout(location = 2) out vec3 fragToLightVector;
layout(location = 3) out vec3 fragToCameraVector;

void main() {
    vec4 worldPosition = evubo.modelMatrix * vec4(position, 1.0);
    gl_Position = ervubo.projectionMatrix * ervubo.viewMatrix * worldPosition;
    fragNormal = (evubo.modelMatrix * vec4(normal, 0.0)).xyz;
    fragTextureCoordinates = textureCoordinates;
    fragToLightVector = ervubo.lightPosition - worldPosition.xyz;
    fragToCameraVector = (inverse(ervubo.viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
}

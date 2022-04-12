#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 modelMatrix;
    mat4 viewMatrix;
    mat4 projectionMatrix;
    vec3 lightPosition;
    vec3 lightColor;
    float reflectivity;
    float shineDamper;
} ubo;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoordinates;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec2 fragTextureCoordinates;
layout(location = 2) out vec3 fragToLightVector;
layout(location = 3) out vec3 fragToCameraVector;

void main() {
    vec4 worldPosition = ubo.modelMatrix * vec4(position, 1.0);
    gl_Position = ubo.projectionMatrix * ubo.viewMatrix * worldPosition;
    fragNormal = (ubo.modelMatrix * vec4(normal, 0.0)).xyz;
    fragTextureCoordinates = textureCoordinates;
    fragToLightVector = ubo.lightPosition - worldPosition.xyz;
    fragToCameraVector = (inverse(ubo.viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
}

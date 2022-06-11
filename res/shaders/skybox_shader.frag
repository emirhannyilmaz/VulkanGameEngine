#version 450

layout(set = 0, binding = 1) uniform SkyboxRendererVertexUniformBufferObject {
    vec3 fogColor;
} srvubo;

layout(set = 1, binding = 1) uniform samplerCube textureSampler;

layout(location = 0) in vec3 fragTextureCoordinates;

layout(location = 0) out vec4 outColor;

const float lowerLimit = 0.0;
const float upperLimit = 30.0;

void main() {
    vec4 originalColor = texture(textureSampler, fragTextureCoordinates);

    float fogFactor = (fragTextureCoordinates.y - lowerLimit) / (upperLimit - lowerLimit);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    outColor = mix(vec4(srvubo.fogColor, 1.0), originalColor, fogFactor);
}

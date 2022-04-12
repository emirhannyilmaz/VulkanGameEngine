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

layout(binding = 1) uniform sampler2D textureSampler;

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTextureCoordinates;
layout(location = 2) in vec3 fragToLightVector;
layout(location = 3) in vec3 fragToCameraVector;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 nNormal = normalize(fragNormal);
    vec3 nToLightVector = normalize(fragToLightVector);

    float dDotProduct = dot(nNormal, nToLightVector);
    dDotProduct = max(dDotProduct, 0.2);
    vec3 diffuseLighting = dDotProduct * ubo.lightColor;

    vec3 nToCameraVector = normalize(fragToCameraVector);
    vec3 reverseToLightVector = -nToLightVector;
    vec3 reflectedLightDirection = reflect(reverseToLightVector, nNormal);

    float sDotProduct = dot(reflectedLightDirection, nToCameraVector);
    sDotProduct = max(sDotProduct, 0.0);
    float sDamped = pow(sDotProduct, ubo.shineDamper);
    vec3 specularLighting = sDamped * ubo.reflectivity * ubo.lightColor;

    outColor = vec4(diffuseLighting, 1.0) * texture(textureSampler, fragTextureCoordinates) + vec4(specularLighting, 1.0);
}

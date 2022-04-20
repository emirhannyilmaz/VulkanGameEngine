#version 450

layout(set = 0, binding = 1) uniform GeneralFragmentUniformBufferObject {
    vec3 lightColor;
} gfubo;

layout(set = 1, binding = 1) uniform EntityFragmentUniformBufferObject {
    float reflectivity;
    float shineDamper;
} efubo;

layout(set = 1, binding = 2) uniform sampler2D textureSampler;

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
    vec3 diffuseLighting = dDotProduct * gfubo.lightColor;

    vec3 nToCameraVector = normalize(fragToCameraVector);
    vec3 reverseToLightVector = -nToLightVector;
    vec3 reflectedLightDirection = reflect(reverseToLightVector, nNormal);

    float sDotProduct = dot(reflectedLightDirection, nToCameraVector);
    sDotProduct = max(sDotProduct, 0.0);
    float sDamped = pow(sDotProduct, efubo.shineDamper);
    vec3 specularLighting = sDamped * efubo.reflectivity * gfubo.lightColor;

    outColor = vec4(diffuseLighting, 1.0) * texture(textureSampler, fragTextureCoordinates) + vec4(specularLighting, 1.0);
}

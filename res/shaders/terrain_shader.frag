#version 450

layout(set = 0, binding = 1) uniform TerrainRendererFragmentUniformBufferObject {
    vec3 lightColor;
    float shadowMapSize;
} trfubo;

layout(set = 1, binding = 1) uniform TerrainFragmentUniformBufferObject {
    float reflectivity;
    float shineDamper;
} tfubo;

layout(set = 0, binding = 2) uniform sampler2D shadowMapSampler;
layout(set = 1, binding = 2) uniform sampler2D textureSampler;

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTextureCoordinates;
layout(location = 2) in vec3 fragToLightVector;
layout(location = 3) in vec3 fragToCameraVector;
layout(location = 4) in vec4 fragPositionInShadowMap;

layout(location = 0) out vec4 outColor;

const int pcfCount = 2;
const float totalTexels = (pcfCount * 2.0 + 1.0) * (pcfCount * 2.0 + 1.0);

void main() {
    float texelSize = 1.0 / trfubo.shadowMapSize;
    float total = 0.0;

    for (int x = -pcfCount; x <= pcfCount; x++) {
        for (int y = -pcfCount; y <= pcfCount; y++) {
            float objectNearestLight = texture(shadowMapSampler, fragPositionInShadowMap.xy + vec2(x, y) * texelSize).r;
            if (fragPositionInShadowMap.z > objectNearestLight + 0.002) {
                total += 1.0;
            }
        }
    }

    total /= totalTexels;

    float lightFactor = 1.0 - (total * fragPositionInShadowMap.w);

    vec3 nNormal = normalize(fragNormal);
    vec3 nToLightVector = normalize(fragToLightVector);

    float dDotProduct = dot(nNormal, nToLightVector);
    dDotProduct = max(dDotProduct, 0.2);
    vec3 diffuseLighting = dDotProduct * trfubo.lightColor;
    diffuseLighting = max(diffuseLighting * lightFactor, 0.3);

    vec3 nToCameraVector = normalize(fragToCameraVector);
    vec3 reverseToLightVector = -nToLightVector;
    vec3 reflectedLightDirection = reflect(reverseToLightVector, nNormal);

    float sDotProduct = dot(reflectedLightDirection, nToCameraVector);
    sDotProduct = max(sDotProduct, 0.0);
    float sDamped = pow(sDotProduct, tfubo.shineDamper);
    vec3 specularLighting = sDamped * tfubo.reflectivity * trfubo.lightColor;

    outColor = vec4(diffuseLighting, 1.0) * texture(textureSampler, fragTextureCoordinates) + vec4(specularLighting, 1.0);
}

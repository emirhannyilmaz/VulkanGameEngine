#version 450

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec4 fragClipSpace;
layout(location = 1) in vec2 fragTextureCoordinates;
layout(location = 2) in vec3 fragToCameraVector;

layout(set = 0, binding = 1) uniform sampler2D reflectionTexture;
layout(set = 0, binding = 2) uniform sampler2D refractionTexture;
layout(set = 0, binding = 3) uniform sampler2D dudvMap;
layout(set = 0, binding = 4) uniform WaterRendererFragmentUniformBufferObject {
    float moveFactor;
} wrfubo;

const float waveStrength = 0.02;

void main() {
    vec2 ndc = (fragClipSpace.xy / fragClipSpace.w) / 2.0 + 0.5;
    vec2 reflectionTextureCoordinates = vec2(ndc.x, -ndc.y);
    vec2 refractionTextureCoordinates = vec2(ndc.x, ndc.y);

    vec2 distortion1 = (texture(dudvMap, vec2(fragTextureCoordinates.x + wrfubo.moveFactor, fragTextureCoordinates.y)).rg * 2.0 - 1.0) * waveStrength;
    vec2 distortion2 = (texture(dudvMap, vec2(-fragTextureCoordinates.x + wrfubo.moveFactor, fragTextureCoordinates.y + wrfubo.moveFactor)).rg * 2.0 - 1.0) * waveStrength;
    vec2 totalDistortion = distortion1 + distortion2;

    reflectionTextureCoordinates += totalDistortion;
    reflectionTextureCoordinates.x = clamp(reflectionTextureCoordinates.x, 0.001, 0.999);
    reflectionTextureCoordinates.y = clamp(reflectionTextureCoordinates.y, -0.999, -0.001);

    refractionTextureCoordinates += totalDistortion;
    refractionTextureCoordinates = clamp(refractionTextureCoordinates, 0.001, 0.999);

    vec4 reflectionColor = texture(reflectionTexture, reflectionTextureCoordinates);
    vec4 refractionColor = texture(refractionTexture, refractionTextureCoordinates);

    vec3 nToCameraVector = normalize(fragToCameraVector);
    float refractiveFactor = dot(nToCameraVector, vec3(0.0, -1.0, 0.0));
    refractiveFactor = pow(refractiveFactor, 0.5);

    outColor = mix(reflectionColor, refractionColor, refractiveFactor);
    outColor = mix(outColor, vec4(0.0, 0.3, 0.5, 1.0), 0.2);
}

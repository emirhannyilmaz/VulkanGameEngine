#version 450

layout(location = 0) out vec4 outColor;
layout(location = 0) in vec4 clipSpace;

layout(set = 0, binding = 1) uniform sampler2D reflectionTexture;
layout(set = 0, binding = 2) uniform sampler2D refractionTexture;

void main() {
    vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;
    ndc.y = 1.0 - ndc.y;
    vec2 reflectionTextureCoordinates = vec2(ndc.x, -ndc.y);
    vec2 refractionTextureCoordinates = vec2(ndc.x, ndc.y);

    vec4 reflectionColor = texture(reflectionTexture, reflectionTextureCoordinates);
    vec4 refractionColor = texture(refractionTexture, refractionTextureCoordinates);

    outColor = mix(reflectionColor, refractionColor, 0.5);
}

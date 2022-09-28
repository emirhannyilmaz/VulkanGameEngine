#version 450

layout(set = 0, binding = 1) uniform ParticleRendererFragmentUniformBufferObject {
    vec3 fogColor;
} prfubo;

layout(set = 1, binding = 1) uniform ParticleFragmentUniformBufferObject {
    float blendFactor;
} pfubo;

layout(set = 1, binding = 2) uniform sampler2D textureSampler;

layout(location = 0) in float fragVisibility;
layout(location = 1) in vec2 fragTextureCoordinates1;
layout(location = 2) in vec2 fragTextureCoordinates2;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 color1 = texture(textureSampler, fragTextureCoordinates1);
    vec4 color2 = texture(textureSampler, fragTextureCoordinates2);

    outColor = mix(color1, color2, pfubo.blendFactor);
//    outColor = mix(vec4(prfubo.fogColor, 1.0), outColor, fragVisibility);
}

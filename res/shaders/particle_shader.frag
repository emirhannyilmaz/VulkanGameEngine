#version 450

layout(set = 0, binding = 1) uniform ParticleRendererFragmentUniformBufferObject {
    vec3 fogColor;
} prfubo;

layout(set = 1, binding = 1) uniform sampler2D textureSampler;

layout(location = 0) in float fragVisibility;
layout(location = 1) in vec2 fragTextureCoordinates;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(textureSampler, fragTextureCoordinates);
    outColor = mix(vec4(prfubo.fogColor, 1.0), outColor, fragVisibility);
}

#version 450

layout(set = 0, binding = 1) uniform ParticleRendererFragmentUniformBufferObject {
    vec3 fogColor;
} prfubo;

layout(location = 0) in float fragVisibility;
layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(1.0);
    outColor = mix(vec4(prfubo.fogColor, 1.0), outColor, fragVisibility);
}

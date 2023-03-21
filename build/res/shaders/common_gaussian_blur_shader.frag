#version 450

float weights[11] = float[] (
    0.0093,
    0.028002,
    0.065984,
    0.121703,
    0.175713,
    0.198596,
    0.175713,
    0.121703,
    0.065984,
    0.028002,
    0.0093
);

layout(set = 0, binding = 1) uniform sampler2D textureSampler;

layout(location = 0) in vec2 fragTextureCoordinates[11];

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(0.0);
    for (int i = 0; i < 11; i++) {
        outColor += texture(textureSampler, fragTextureCoordinates[i]) * weights[i];
    }
    outColor.a = 1.0;
}

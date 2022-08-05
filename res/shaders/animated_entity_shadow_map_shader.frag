#version 450

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 2) uniform sampler2D textureSampler;

layout(location = 0) in vec2 fragTextureCoordinates;

void main() {
    float alpha = texture(textureSampler, fragTextureCoordinates).a;
    if (alpha < 0.5) {
        discard;
    }

    outColor = vec4(1.0);
}

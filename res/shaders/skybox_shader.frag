#version 450

layout(set = 1, binding = 1) uniform samplerCube textureSampler;

layout(location = 0) in vec3 fragTextureCoordinates;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(textureSampler, fragTextureCoordinates);
}

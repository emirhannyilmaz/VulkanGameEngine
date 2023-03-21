#version 450

vec3 vertices[6] = vec3[] (
    vec3(-1.0, 1.0, 0.0),
    vec3(-1.0, -1.0, 0.0),
    vec3(1.0, -1.0, 0.0),
    vec3(1.0, -1.0, 0.0),
    vec3(1.0, 1.0, 0.0),
    vec3(-1.0, 1.0, 0.0)
);

layout(set = 0, binding = 0) uniform VerticalGaussianBlurVertexUniformBufferObject {
    float imageHeight;
} vgbvubo;

layout(location = 0) out vec2 fragTextureCoordinates[11];

void main() {
    gl_Position = vec4(vertices[gl_VertexIndex], 1.0);
    vec2 textureCoordinates = vec2(vertices[gl_VertexIndex].x / 2.0 + 0.5, vertices[gl_VertexIndex].y / 2.0 + 0.5);
    float pixelHeight = 1.0 / vgbvubo.imageHeight;

    for (int i = -5; i <= 5; i++) {
        fragTextureCoordinates[i + 5] = textureCoordinates + vec2(pixelHeight * i, 0.0);
    }
}

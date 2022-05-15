#version 450

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec4 fragClipSpace;
layout(location = 1) in vec2 fragTextureCoordinates;
layout(location = 2) in vec3 fragToCameraVector;
layout(location = 3) in vec3 fragFromLightVector;

layout(set = 0, binding = 1) uniform sampler2D reflectionTexture;
layout(set = 0, binding = 2) uniform sampler2D refractionTexture;
layout(set = 0, binding = 3) uniform sampler2D dudvMap;
layout(set = 0, binding = 4) uniform sampler2D normalMap;
layout(set = 0, binding = 5) uniform sampler2D depthTexture;
layout(set = 0, binding = 6) uniform WaterRendererFragmentUniformBufferObject {
    float moveFactor;
    vec3 lightColor;
    float nearPlane;
    float farPlane;
} wrfubo;

layout(set = 1, binding = 1) uniform WaterTileFragmentUniformBufferObject {
    float reflectivity;
    float shineDamper;
} wtfubo;

const float waveStrength = 0.02;

void main() {
    vec2 ndc = (fragClipSpace.xy / fragClipSpace.w) / 2.0 + 0.5;
    vec2 reflectionTextureCoordinates = vec2(ndc.x, -ndc.y);
    vec2 refractionTextureCoordinates = vec2(ndc.x, ndc.y);

    float depth = texture(depthTexture, refractionTextureCoordinates).r;
    float floorDistance = wrfubo.nearPlane * wrfubo.farPlane / (wrfubo.farPlane + depth * (wrfubo.nearPlane - wrfubo.farPlane));

    depth = gl_FragCoord.z;
    float waterDistance = wrfubo.nearPlane * wrfubo.farPlane / (wrfubo.farPlane + depth * (wrfubo.nearPlane - wrfubo.farPlane));
    float waterDepth = floorDistance - waterDistance;

    vec2 distortedTextureCoordinates = texture(dudvMap, vec2(fragTextureCoordinates.x + wrfubo.moveFactor, fragTextureCoordinates.y)).rg * 0.1;
    distortedTextureCoordinates = fragTextureCoordinates + vec2(distortedTextureCoordinates.x, distortedTextureCoordinates.y + wrfubo.moveFactor);
    vec2 totalDistortion = (texture(dudvMap, distortedTextureCoordinates).rg * 2.0 - 1.0) * waveStrength * clamp(waterDepth / 20.0, 0.0, 1.0);

    reflectionTextureCoordinates += totalDistortion;
    reflectionTextureCoordinates.x = clamp(reflectionTextureCoordinates.x, 0.001, 0.999);
    reflectionTextureCoordinates.y = clamp(reflectionTextureCoordinates.y, -0.999, -0.001);

    refractionTextureCoordinates += totalDistortion;
    refractionTextureCoordinates = clamp(refractionTextureCoordinates, 0.001, 0.999);

    vec4 reflectionColor = texture(reflectionTexture, reflectionTextureCoordinates);
    vec4 refractionColor = texture(refractionTexture, refractionTextureCoordinates);

    vec4 normalMapColor = texture(normalMap, distortedTextureCoordinates);
    vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b * 3.0 * -1.0, normalMapColor.g * 2.0 - 1.0);
    normal = normalize(normal);

    vec3 nToCameraVector = normalize(fragToCameraVector);
    float refractiveFactor = dot(nToCameraVector, normal);
    refractiveFactor = pow(refractiveFactor, 1.0);

    vec3 reflectedLightDirection = reflect(normalize(fragFromLightVector), normal);
    float sDotProduct = dot(reflectedLightDirection, nToCameraVector);
    sDotProduct = max(sDotProduct, 0.0);
    float sDamped = pow(sDotProduct, wtfubo.shineDamper);
    vec3 specularLighting = sDamped * wtfubo.reflectivity * wrfubo.lightColor * clamp(waterDepth / 5.0, 0.0, 1.0);

    outColor = mix(reflectionColor, refractionColor, refractiveFactor);
    outColor = mix(outColor, vec4(0.0, 0.3, 0.5, 1.0), 0.2) + vec4(specularLighting, 0.0);
    outColor.a = clamp(waterDepth / 5.0, 0.0, 1.0);
}

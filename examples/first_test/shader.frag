#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (set = 0, binding = 1) uniform texture2D tex;
layout (set = 0, binding = 2) uniform sampler samp;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 inTexCoords;
layout(location = 2) in vec3 LightingColor;


layout(location = 0) out vec4 outColor;

void main() {

    float ambientStrength = 0.1;

    vec4 resColor = texture(sampler2D(tex, samp), inTexCoords);
    vec4 result = resColor * vec4(LightingColor, 1.0f);
    outColor = result;
}
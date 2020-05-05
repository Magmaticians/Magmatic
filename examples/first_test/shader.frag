#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (set = 0, binding = 1) uniform texture2D tex;
layout (set = 0, binding = 2) uniform sampler samp;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 inTexCoords;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 resColor = texture(sampler2D(tex, samp), inTexCoords);
    if ((inTexCoords.x < 0.01f) || (0.99f < inTexCoords.x)
    || (inTexCoords.y < 0.01f) || (0.99f < inTexCoords.y))
    {
        resColor *= vec4(0.1f, 0.1f, 0.1f, 1.0f);
    }

    outColor = resColor;
}
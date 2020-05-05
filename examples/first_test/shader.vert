#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(push_constant) uniform PER_OBJECT
{
    uint textureID;
    uint samplerID;
    mat4 matrix;
}pc;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 inTexCoord;


layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 LightingColor;


void main() {
    vec3 lightColor = vec3(1, 1, 1);
    vec3 viewPos = vec3(0, 0, 0);
    vec3 lightPos = vec3(1.0, 1.0, 1.0);


    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);

    vec3 Normal = mat3(transpose(inverse(ubo.model))) * normal;
    vec3 Position = vec3(ubo.model * vec4(inPosition, 1.0));

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 1.0; // this is set higher to better show the effect of Gouraud shading
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    LightingColor = ambient + diffuse + specular;
    fragColor = inColor;

    fragTexCoord = inTexCoord;
}
#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Color;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

layout(push_constant) uniform PushConsts {
    mat4 model;
} pushConsts;

void main() {
    gl_Position = ubo.proj * ubo.view * pushConsts.model * vec4(Position, 1.0);
    fragColor = Color;
    fragTexCoord = texCoord;
}
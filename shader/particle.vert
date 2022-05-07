#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in float height;

out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec3 aPos = vec3(pos[0], height, pos[1]);
    FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
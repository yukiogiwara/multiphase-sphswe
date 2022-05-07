#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in float height;
layout (location = 2) in vec3 color;

out vec3 frag_pos;
out vec3 frag_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec3 aPos = vec3(pos[0], height, pos[1]);
    frag_pos = vec3(model * vec4(aPos, 1.0));
    frag_color = color;
    gl_Position = projection * view * vec4(frag_pos, 1.0);
}
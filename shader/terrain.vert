#version 330 core

layout (location = 0) in vec3 pos;

out vec3 frag_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    frag_pos = vec3(model * vec4(pos, 1.0));
    gl_Position = projection * view * vec4(frag_pos, 1.0);
}
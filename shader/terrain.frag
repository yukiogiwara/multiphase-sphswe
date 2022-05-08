#version 330 core

out vec4 FragColor;

in vec3 frag_pos;

void main() {
    FragColor = vec4(vec3(0.5f), 1.0f);
}
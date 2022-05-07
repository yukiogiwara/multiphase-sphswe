#version 330 core

out vec4 FragColor;

in vec3 frag_pos;
in vec3 frag_color;

void main() {
    vec2 coord = gl_PointCoord - vec2(0.5);
    if(length(coord) > 0.5) discard;
    FragColor = vec4(frag_color, 1.0f);
}
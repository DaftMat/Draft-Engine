#version 410
in vec3 icolor;

out vec4 color;

void main() {
    color = vec4(icolor, 1.0);
}

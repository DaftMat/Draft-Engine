#version 410 core
in vec3 position;
in vec3 normal;

out vec4 color;

void main()
{
    float error = (1.0 - length(position)) * 20;
    color = vec4(error, 1.0-error, 0.0, 1.0);
}
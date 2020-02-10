#version 410 core
layout (location = 0) in vec3 iposition;
layout (location = 1) in vec3 inormal;
layout (location = 2) in vec2 texcoord;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec3 position;
out vec3 normal;

void main()
{
    // Note that we read the multiplication from right to left
    gl_Position = projection * view * model * vec4(iposition, 1.0f);
    position = iposition;
    normal = inormal;
}
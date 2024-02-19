#version 330 core

layout (location = 0) in vec3 position;
uniform mat4 mvp;
out vec3 color;

void main()
{
    gl_Position = mvp * vec4(position, 1.0);
    color = vec3(0.5, 0.8, 0.2);
}
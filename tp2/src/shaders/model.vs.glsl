#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(position.x, position.y, position.z, 1.0);
}

#version 330 core

out vec4 FragColor;

uniform vec3 outputColor;

void main()
{
    FragColor = vec4(outputColor, 1.0);
}
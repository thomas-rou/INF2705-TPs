#version 330 core

in vec3 color;
out vec4 FragColor;
uniform vec3 u_color;

void main()
{
    FragColor = vec4(color * u_color, 1.0f);
}
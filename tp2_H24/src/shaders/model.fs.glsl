#version 330 core

in vec2 vTex;

uniform sampler2D tex;

out vec4 FragColor;

void main()
{
    FragColor = texture(tex, vTex);
    if (FragColor.a < 0.3) discard;
}
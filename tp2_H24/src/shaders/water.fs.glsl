#version 330 core

in vec2 vTexture;

uniform sampler2D tex;
uniform float dtime;

out vec4 FragColor;

void main()
{
    float dx = 0.1 * dtime;
    FragColor = texture(tex, vTexture + vec2(dx, 0.0));
    if (FragColor.a < 0.3) discard;
}
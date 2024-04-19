#version 330 core

out vec4 FragColor;


uniform sampler2D textureSampler;

in ATTRIB_GS_OUT
{
    vec4 color;
    vec2 texCoords;
} attribIn;

void main() {
    FragColor = attribIn.color;
    vec4 texel = texture(textureSampler, attribIn.texCoords);
    FragColor = mix(texel, attribIn.color, 0.5);
    if (FragColor.a <= 0.05) discard;
}
#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;


in ATTRIB_VS_OUT
{
    vec4 color;
    vec2 size;
} attribIn[];

out ATTRIB_GS_OUT
{
    vec4 color;
    vec2 texCoords;
} attribOut;

uniform mat4 projection;

void main() {
    vec2 offsets[4] = {
        vec2(-0.5, -0.5),
        vec2( 0.5, -0.5),
        vec2(-0.5,  0.5),
        vec2( 0.5,  0.5)
    };

    for (int i = 0; i < 4; i++) {
        setAttributes(offsets[i]);
        EmitVertex();
    }
    EndPrimitive();
}

void setAttributes(vec2 offset) {
    gl_Position = projection * (gl_in[0].gl_Position + vec4(offset * attribIn[0].size, 0.0, 0.0));
    attribOut.color = attribIn[0].color;
    attribOut.texCoords = offset + 0.5;
}
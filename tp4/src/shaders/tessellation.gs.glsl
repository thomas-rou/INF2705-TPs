#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;


in ATTRIB_TES_OUT
{
    float height;
    vec2 texCoords;
    vec4 patchDistance;
} attribIn[];

out ATTRIB_GS_OUT
{
    float height;
    vec2 texCoords;
    vec4 patchDistance;
    vec3 barycentricCoords;
} attribOut;

void main() {
    const vec3 bary[3] = {
        vec3(1.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 0.0f, 1.0f)
    };

    int length = gl_in.length();
    for (int i = 0; i < length; ++i) {
        setAttributes(i, bary[i]);
        EmitVertex();
    }
    EndPrimitive();
}

void setAttributes(int i, vec3 bary) {
    gl_Position = gl_in[i].gl_Position;
    attribOut.height = attribIn[i].height;
    attribOut.texCoords = attribIn[i].texCoords;
    attribOut.patchDistance = attribIn[i].patchDistance;
    attribOut.barycentricCoords = bary;
}
#version 400 core

layout(quads) in;

/*
in Attribs {
    vec4 couleur;
} AttribsIn[];*/


out ATTRIB_TES_OUT
{
    float height;
    vec2 texCoords;
    vec4 patchDistance;
} attribOut;

uniform mat4 mvp;

uniform sampler2D heighmapSampler;

vec4 interpole( vec4 v0, vec4 v1, vec4 v2, vec4 v3 )
{
    // mix( x, y, f ) = x * (1-f) + y * f.
    // TODO
    vec4 v01 = mix(v0, v1, gl_TessCoord.x);
    vec4 v23 = mix(v2, v3, gl_TessCoord.x);

    // Interpolez entre les résultats
    return mix(v01, v23, gl_TessCoord.y);
}


const float PLANE_SIZE = 256.0f;

void main()
{
	// TODO
    vec4 position = interpole(
        gl_in[0].gl_Position, 
        gl_in[1].gl_Position,
        gl_in[2].gl_Position, 
        gl_in[3].gl_Position
    );
    
    vec2 heightTexCoords = position.xz / PLANE_SIZE + 0.5f;
    float height = texture(heighmapSampler, heightTexCoords / 4.0f).r;
    attribOut.height = height;
    gl_Position = mvp * position;
    attribOut.texCoords = gl_TessCoord.xy * 2.0f;
    attribOut.patchDistance = vec4(
        gl_TessCoord.x, 
        gl_TessCoord.y, 
        1 - gl_TessCoord.x, 
        1 - gl_TessCoord.y
    );
}

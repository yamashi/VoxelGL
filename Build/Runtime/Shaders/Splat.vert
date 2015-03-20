#version 150 core

in vec2 in_Vertex;
in vec2 in_TexCoord0;


uniform mat4 projection;
uniform mat4 modelview;

out vec2 coordTexture;

void main()
{
    gl_Position = vec4(in_Vertex, 0.0, 1.0);

    coordTexture = in_TexCoord0;
}

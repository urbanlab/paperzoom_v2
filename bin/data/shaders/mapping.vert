#version 150

in vec4 position;
in vec2 texcoord;

out vec2 vs_texcoord;


void main()
{
    vs_texcoord = texcoord;
    gl_Position = vec4(position.xyz, 1.f);
}

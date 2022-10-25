#version 150

in vec2 vs_texcoord;

uniform sampler2DRect tex;
uniform ivec2 tex_resolution;

out vec4 frag_color;

float map(float value, float istart, float istop, float ostart, float ostop)
{
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

void main()
{
    vec2 texcoord = vec2(vs_texcoord.x, vs_texcoord.y);
    vec4 color = texture(tex, texcoord * tex_resolution);

    if ( color.a == 0 )
    {
        color = vec4(1,0,0,1);
    }
    
    frag_color = color;
}

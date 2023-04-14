#version 150

in vec2 vs_texcoord;

uniform sampler2DRect tex;
uniform ivec2 tex_resolution;

uniform sampler2DRect img1;
uniform sampler2DRect img2;
uniform sampler2DRect img3;
uniform sampler2DRect img4;
uniform sampler2DRect img5;
uniform sampler2DRect img6;
uniform sampler2DRect img7;
uniform sampler2DRect img8;
uniform sampler2DRect img9;
uniform ivec2 img_resolution;

uniform int n_steps;

out vec4 frag_color;

float map(float value, float istart, float istop, float ostart, float ostop)
{
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

void main()
{
    vec2 texcoord = vec2(vs_texcoord.x, vs_texcoord.y);
    vec4 color = texture(tex, texcoord * tex_resolution);
    
    int s = int(round(map(color.r, 0.f, 1.f, 0.f, float(n_steps))));
    
    texcoord = vec2( (texcoord.x*3-1.25), 1.f-(texcoord.y*3-1.25) );
    
    vec4 img_color;
    if ( s == 0 ) { img_color = texture(img1, texcoord * img_resolution); }
    if ( s == 1 ) { img_color = texture(img2, texcoord * img_resolution); }
    if ( s == 2 ) { img_color = texture(img3, texcoord * img_resolution); }
    if ( s == 3 ) { img_color = texture(img4, texcoord * img_resolution); }
    if ( s == 4 ) { img_color = texture(img5, texcoord * img_resolution); }
    if ( s == 5 ) { img_color = texture(img6, texcoord * img_resolution); }
    if ( s == 6 ) { img_color = texture(img7, texcoord * img_resolution); }
    if ( s == 7 ) { img_color = texture(img8, texcoord * img_resolution); }
    if ( s == 8 ) { img_color = texture(img9, texcoord * img_resolution); }

    vec3 out_color = img_color.xyz;

    
    frag_color = vec4(vec3(out_color), 1.f);
}

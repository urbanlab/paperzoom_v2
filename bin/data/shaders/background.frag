#version 150

in vec2 vs_texcoord;

uniform sampler2DRect tex;
uniform ivec2 tex_resolution;

uniform int n_steps;
uniform float min_depth;
uniform float max_depth;

//uniform vec4 color_0, color_1, color_2, color_3, color_4, color_5, color_6, color_7, color_8, color_9, color_10, color_11;

uniform sampler2DRect img0;
uniform sampler2DRect img1;
uniform sampler2DRect img2;
uniform sampler2DRect img3;
uniform sampler2DRect img4;
uniform sampler2DRect img5;
uniform sampler2DRect img6;
uniform sampler2DRect img7;
uniform sampler2DRect img8;
uniform sampler2DRect img9;
uniform sampler2DRect img10;
uniform sampler2DRect img11;

out vec4 frag_color;

float map(float value, float istart, float istop, float ostart, float ostop)
{
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

void main()
{
    vec2 texcoord = vec2(1.f - vs_texcoord.x, 1.f - vs_texcoord.y);
    vec4 color = texture(tex, texcoord * tex_resolution);
    
//    if ( color.r < min_depth || color.r > max_depth )
//    {
//        color = vec4(0);
//    }
//    else
//    {
//        color = vec4(vec3(map(color.r, min_depth, max_depth, 0.f, 1.f)), 1.f);
//    }
//
    vec4 out_color = color;
//
//    if ( out_color.a > 0 )
//    {
//        int s = int(round(map(color.r, 0.f, 1.f, 0.f, float(n_steps))));
//
//        vec4 img_color;
//        if ( s == 0 ) { img_color = texture(img0, texcoord * tex_resolution); }
//        if ( s == 1 ) { img_color = texture(img1, texcoord * tex_resolution); }
//        if ( s == 2 ) { img_color = texture(img2, texcoord * tex_resolution); }
//        if ( s == 3 ) { img_color = texture(img3, texcoord * tex_resolution); }
//        if ( s == 4 ) { img_color = texture(img4, texcoord * tex_resolution); }
//        if ( s == 5 ) { img_color = texture(img5, texcoord * tex_resolution); }
//        if ( s == 6 ) { img_color = texture(img6, texcoord * tex_resolution); }
//        if ( s == 7 ) { img_color = texture(img7, texcoord * tex_resolution); }
//        if ( s == 8 ) { img_color = texture(img8, texcoord * tex_resolution); }
//        if ( s == 9 ) { img_color = texture(img9, texcoord * tex_resolution); }
//        if ( s == 10 ) { img_color = texture(img10, texcoord * tex_resolution); }
//        if ( s == 11 ) { img_color = texture(img11, texcoord * tex_resolution); }
//
//        out_color = img_color;
//    }
//    else
//    {
        out_color = texture(img0, texcoord * tex_resolution);
//    }

    frag_color = out_color;
}

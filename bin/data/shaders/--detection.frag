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
    // gl_FragCoord contains the window relative coordinate for the fragment.
    // we use gl_FragCoord.x position to control the red color value.
    // we use gl_FragCoord.y position to control the green color value.
    // please note that all r, g, b, a values are between 0 and 1.
    
//    float windowWidth = 1920.f;
//    float windowHeight = 1080.f;
    
    float r = vs_texcoord.x; //gl_FragCoord.x / windowWidth;
    float g = vs_texcoord.y; //gl_FragCoord.y / windowHeight;
    float b = 1.0;
    float a = 1.0;
    
    vec2 texcoord = vec2(1.f - vs_texcoord.x, 1.f - vs_texcoord.y);
    vec4 color = texture(tex, texcoord * tex_resolution);
    color = vec4(vec3(map(color.r, (1.f/12.f)*2, (1.f/12.f)*6, 0.f, 1.f)), 1.f);
        
    vec3 step_colors[12] = vec3[12]
    (
        vec3(255, 0, 0) / vec3(255),
        vec3(0, 255, 0) / vec3(255),
        vec3(0, 0, 255) / vec3(255),
        vec3(255, 255, 0) / vec3(255),
        vec3(0, 255, 255) / vec3(255),
        vec3(255, 0, 255) / vec3(255),
        vec3(96, 192, 0) / vec3(255),
        vec3(0, 96, 192) / vec3(255),
        vec3(96, 0, 192) / vec3(255),
        vec3(192, 96, 0) / vec3(255),
        vec3(0, 192, 96) / vec3(255),
        vec3(192, 0, 96)  / vec3(255)
    );
    
    int s = int(round(map(color.r, 0.f, 1.f, 0.f, float(n_steps))));
    
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
//    if ( s < 4 ) { img_color = vec4(0); }
    
    float out_color = map(float(s), 0.f, float(n_steps), 0.f, 1.f);
//    vec3 out_color = step_colors[s];
//    vec3 out_color = img_color.xyz;

    
    frag_color = vec4(vec3(out_color), 1.f);//vec4(r, g, b, a);
}

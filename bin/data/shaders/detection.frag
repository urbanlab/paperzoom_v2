#version 150

in vec2 vs_texcoord;

uniform sampler2DRect tex;
uniform ivec2 tex_resolution;

uniform int n_steps;

uniform vec4 color_0, color_1, color_2, color_3, color_4, color_5, color_6, color_7, color_8, color_9, color_10, color_11;

uniform float min_depth;
uniform float max_depth;
//
//uniform float scale;
//uniform int tex_x;
//uniform int tex_y;

out vec4 frag_color;

float map(float value, float istart, float istop, float ostart, float ostop)
{
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

void main()
{
    vec2 texcoord = vec2(1.f - vs_texcoord.x, 1.f - vs_texcoord.y)*tex_resolution;
    vec4 color = texture(tex, texcoord);
    
    if ( color.r < min_depth || texcoord.x < 0 || texcoord.y >= tex_resolution.y)
    {
        color = vec4(0);
    }
    else if ( color.r > max_depth )
    {
        color = vec4(1);
    }
    else
    {
//        float map_scale = map(color.r, min_depth, max_depth, 0.f, scale );
//        int map_tex_x = int(round(map(color.r, min_depth, max_depth, 0.f, float(tex_x) )));
//        int map_tex_y = int(round(map(color.r, min_depth, max_depth, 0.f, float(tex_y) )));
//
//        texcoord *= tex_resolution;
//
//        texcoord -= vec2(0.5f);
//        texcoord *= (1.f/map_scale);
//        texcoord += vec2(0.5f);
//
//        texcoord *= tex_resolution;
//        texcoord += vec2(map_tex_x-tex_resolution.x/2,map_tex_x-tex_resolution.y/2);
//
//        color = texture(tex, texcoord);
        
        color = vec4(vec3(map(color.r, min_depth, max_depth, 0.f, 1.f)), 1.f);
    }
    
    vec4 out_color = color;
    
    if ( out_color.a > 0 )
    {
        int s = int(round(map(color.r, 0.f, 1.f, 0.f, float(n_steps))));
        
        vec3 step_colors[12] = vec3[12]
        (
            vec3(color_0), vec3(color_1),vec3(color_2),vec3(color_3),
            vec3(color_4), vec3(color_5),vec3(color_6),vec3(color_7),
            vec3(color_8), vec3(color_9),vec3(color_10),vec3(color_11)
        );

        out_color = vec4(step_colors[s], 1.f);
    }

    frag_color = out_color;
}

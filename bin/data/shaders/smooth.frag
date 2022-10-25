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
    int c = int(round(map(color.r, 0.f, 1.f, 0.f, 255.f)));
    float a = color.r;
    
    int inner_thresh = 0;
    int outer_thresh = 0;
    float a_thresh = 0.01;
    
    if (a <= a_thresh)
    {
        int depth_mat_value[24];
        int depth_mat_count[24];
        for ( int i = 0; i < 24; i++ )
        {
            depth_mat_value[i] = 0;
            depth_mat_count[i] = 0;
        }
        
        int inner_count = 0;
        int outer_count = 0;
        
        for ( int i = -2; i <= 2; i++ )
        {
            for ( int j = -2; j <= 2; j++ )
            {
                if ( i != 0 || j != 0 )
                {
                    vec2 sub_tex = (texcoord * tex_resolution) + vec2(i,j);
                        
                    if (sub_tex.x >= 0 && sub_tex.x < tex_resolution.x ){
                    if (sub_tex.y >= 0 && sub_tex.y < tex_resolution.y )
                    {
                        vec4 sub_color = texture(tex,sub_tex);

                        if (sub_color.r >= a_thresh )
                        {
                            int sub_c = int(round(map(sub_color.r, 0.f, 1.f, 0.f, 255.f)));

                            bool found = false;
                            for ( int k = 0; k < 24 && !found; k++ )
                            {
                                if ( depth_mat_value[k] == 0 )
                                {
                                    depth_mat_value[k] = sub_c;
                                    depth_mat_count[k]++;
                                    found = true;
//                                    break;
                                }
                                else if ( depth_mat_value[k] == sub_c )
                                {
                                    depth_mat_count[k]++;
                                    found = true;
                                    //break;
                                }
                            }
                            
                            if ( i != 2 && i != -2 && j != 2 && j != -2 )
                            {
                                inner_count++;
                            }
                            else
                            {
                                outer_count++;
                            }
                        }
                    }}
                }
            }
        }
        
        if ( inner_count >= inner_thresh || outer_count >= outer_thresh )
        {
            int frequency = 0;
            int depth = 0;

            bool found = false;
            for ( int k = 0; k < 255 && !found; k++ )
            {
                if ( depth_mat_value[k] == 0 )
                {
                    found = true;
//                    break;
                }
                if ( depth_mat_count[k] > frequency )
                {
                    depth = depth_mat_value[k];
                    frequency = depth_mat_count[k];
                }
            }

            float new_c = map(depth, 0.f, 255.f, 0.f, 1.f);
            color = vec4(vec3(new_c),1.f);
//            color = vec4(vec3(1,0,0),1);
        }
    }
    
    frag_color = color;
}

#version 150

in vec2 vs_texcoord;

uniform sampler2DRect tex1;
uniform sampler2DRect tex2;
uniform sampler2DRect tex3;
uniform sampler2DRect tex4;
uniform sampler2DRect tex5;
uniform sampler2DRect tex6;
uniform sampler2DRect tex7;
uniform sampler2DRect tex8;
uniform sampler2DRect tex9;
uniform sampler2DRect tex10;

uniform int n_textures;

uniform ivec2 tex_resolution;

out vec4 frag_color;

void main()
{
    vec2 texcoord = vec2(1.0-vs_texcoord.x, 1.0-vs_texcoord.y);

    vec4 color = vec4(vec3(0),1);
    int count = 0;
  
    for ( int i = -1; i <= 1; i++ )
    {
        for ( int j = -1; j <= 1; j++ )
        {
            for ( int k = 0; k < n_textures; k++ )
            {
                vec4 tex_color;
                int pound = (9-k)+1;
                
                if ( k == 0 ) { tex_color = texture(tex1, (texcoord * tex_resolution) + vec2(i,j) ); }
                if ( k == 1 ) { tex_color = texture(tex2, (texcoord * tex_resolution) + vec2(i,j) ); }
                if ( k == 2 ) { tex_color = texture(tex3, (texcoord * tex_resolution) + vec2(i,j) ); }
                if ( k == 3 ) { tex_color = texture(tex4, (texcoord * tex_resolution) + vec2(i,j) ); }
                if ( k == 4 ) { tex_color = texture(tex5, (texcoord * tex_resolution) + vec2(i,j) ); }
                if ( k == 5 ) { tex_color = texture(tex6, (texcoord * tex_resolution) + vec2(i,j) ); }
                if ( k == 6 ) { tex_color = texture(tex7, (texcoord * tex_resolution) + vec2(i,j) ); }
                if ( k == 7 ) { tex_color = texture(tex8, (texcoord * tex_resolution) + vec2(i,j) ); }
                if ( k == 8 ) { tex_color = texture(tex9, (texcoord * tex_resolution) + vec2(i,j) ); }
                if ( k == 9 ) { tex_color = texture(tex10, (texcoord * tex_resolution) + vec2(i,j) ); }
                
                color += vec4( vec3(tex_color)*pound, 0);
                count += pound;
            }

        }
    }
    

    
    color = vec4( vec3(color)/count, 1);
    frag_color = color;
}

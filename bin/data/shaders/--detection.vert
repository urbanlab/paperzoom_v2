//#version 150
//
//uniform mat4 modelViewProjectionMatrix;
//
//in vec4 position;
//
//void main(){
//    gl_Position = modelViewProjectionMatrix * position;
//}

#version 150

in vec4 position;
in vec2 texcoord;

out vec2 vs_texcoord;

float map(float value, float istart, float istop, float ostart, float ostop)
{
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

void main()
{
    vs_texcoord = texcoord;
//    float x = map(position.x, -1, 1, -0.5, 0.5);
//    float y = map(position.y, -1, 1, -0.5, 0.5);

//    gl_Position = vec4(x, y, position.z, 1.f);
    gl_Position = vec4(position.xyz, 1.f);
}

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextureCoord;


uniform bool use_texture;
uniform bool use_normal;

uniform mat4 view;
uniform float scale;
uniform vec3  delta;

out vec2 texCoord;
void main()
{
    texCoord = aTextureCoord;
    vec4 pos = vec4(scale*aPos+delta,1);
    gl_Position = view*pos;
}
#version 330 core
in vec2 texCoord;

uniform bool use_texture;
uniform sampler2D texture0;

void main()
{
   if(use_texture) gl_FragColor = texture(texture0, texCoord);
   else gl_FragColor = vec4(1,1,1,0);
}
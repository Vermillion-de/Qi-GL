#version 330 core
in vec2 texCoord;
uniform sampler2D texture0;

void main()
{
   // gl_FragColor = vec4(0,0,0,0);
   gl_FragColor = texture(texture0, texCoord);
}
#version 330 core

in vec3 vColorOut;  // 顶点着色器传递的颜色
out vec4 fColor;    // 片元输出像素的颜色

uniform sampler2D texture

void main()
{
    fColor = vec4(vColorOut, 1.0);
}

#version 330 core

in vec3 vPosition;  // cpu传入的顶点坐标
in vec3 vColor;     // cpu传入的顶点颜色

out vec3 vColorOut; // 向片元着色器传递顶点颜色

void main()
{
    gl_Position = vec4(vPosition, 1.0); // 指定ndc坐标
    vColorOut = vColor; // 这个颜色经过线性插值生成片元颜色
}

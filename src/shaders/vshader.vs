#version 330 core

in vec3 vPosition;  // cpu传入的顶点坐标
in vec2 tPosition   // cpu传入的纹理坐标

out vec3 tPosition; // 向片元着色器传递纹理坐标

void main()
{
    gl_Position = vec4(vPosition, 1.0); // 指定ndc坐标
    tPosition = tPosition; // 这个颜色经过线性插值生成片元颜色
}



#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <functional>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
// 读取文件并且返回一个长字符串表示文件内容
std::string readShaderFile(std::string filepath)
{
    std::string res, line;
    std::ifstream fin(filepath);
    if (!fin.is_open())
    {
        std::cout << "文件 " << filepath << " 打开失败" << std::endl;
        exit(-1);
    }
    while (std::getline(fin, line)) res += line + '\n';
    fin.close();
    return res;
}

// 获取着色器对象
GLuint getShaderProgram(std::string fshader, std::string vshader)
{
    // 读取shader源文件
    std::string vSource = readShaderFile(vshader);
    std::string fSource = readShaderFile(fshader);
    const char* vpointer = vSource.c_str();
    const char* fpointer = fSource.c_str();

    // 容错
    std::function<void(GLuint, std::string name)> checkout = [](GLuint shaderID, std::string name){
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);   // 错误检测
        if (!success)
        {
            glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
            std::cout << name << "编译错误\n" << infoLog << std::endl;
            exit(-1);
        }
    };
   


    // 创建并编译顶点着色器
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar**)(&vpointer), NULL);
    glCompileShader(vertexShader);
    checkout(vertexShader,"顶点着色器");
    

    // 创建并且编译片段着色器
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar**)(&fpointer), NULL);
    glCompileShader(fragmentShader);
    checkout(fragmentShader,"片元着色器");

    // 链接两个着色器到program对象
    GLuint shaderProgram = glCreateProgram(); 
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
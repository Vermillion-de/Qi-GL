#include <fstream>
#include <iostream>
#include <string>

class Shader
{
private:
    std::string name;
    std::string program;
    unsigned int shader_ID;
public:
    Shader(){};
    Shader(std::string shader_path){};
    ~Shader(){};
public:
    void use_shader();
};

Shader::Shader(std::string shader_path)
{

}

void Shader::use_shader()
{

}


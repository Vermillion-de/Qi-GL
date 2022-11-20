#pragma once

#include <glad.h>
#include <GL/gl.h>

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>

class Shader
{
public:
    Shader(std::string vs_path, std::string fs_path):vshader_path(vs_path), fshader_path(fs_path){};
    ~Shader(){};

public: 
    unsigned int shader_ID;
    unsigned int vshader_ID;
    unsigned int fshader_ID;    
    std::string vshader_path;
    std::string fshader_path;
public:
    void load(){
        // read source from file
        std::ifstream vshader_file(vshader_path);
        std::ifstream fshader_file(fshader_path);
        std::stringstream vshader_code, fshader_code;
        vshader_code << vshader_file.rdbuf();
        fshader_code << fshader_file.rdbuf();
        vshader_file.close();
        fshader_file.close();
        std::string vs_temp = vshader_code.str();
        std::string fs_temp = fshader_code.str();
        const char* vshader_source = vs_temp.c_str();
        const char* fshader_source = fs_temp.c_str();

        // a check program for logging
        std::function<void(unsigned int, GLenum)> checkProgram = [](unsigned int Program_ID, GLenum flag){
            int success;
            glGetShaderiv(Program_ID, flag, &success);
            if (!success)
            {
            char infoLog[512];
                glGetShaderInfoLog(Program_ID, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::ID("<< Program_ID << ")::FLAG(" << flag << ")" << "::FAILED!\n" << infoLog << std::endl;
            }
        };

        //compile and attach them    
        vshader_ID = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vshader_ID, 1, &vshader_source, NULL);
        glCompileShader(vshader_ID);
        checkProgram(vshader_ID, GL_COMPILE_STATUS);

        fshader_ID = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fshader_ID, 1, &fshader_source, NULL);
        glCompileShader(fshader_ID);
        checkProgram(fshader_ID, GL_COMPILE_STATUS);

        shader_ID = glCreateProgram();
        glAttachShader(shader_ID, vshader_ID);
        glAttachShader(shader_ID, fshader_ID);
        glLinkProgram(shader_ID);
        checkProgram(shader_ID, GL_LINK_STATUS);
    };
    // void reload();
    void bind(){ 
        glUseProgram(shader_ID); 
    };
public:

    void set_1i(const char* name, int x){
        int i_ID = glGetUniformLocation(shader_ID, name);
        glUniform1i(i_ID, x); 
    };

    void set_1f(const char* name, float v){
        int v_ID = glGetUniformLocation(shader_ID, name);
        glUniform1f(v_ID, v); 
    };

    void set_3f(const char* name, float x, float y, float z){
        int value_ID = glGetUniformLocation(shader_ID, name);
        glUniform3f(value_ID,x, y, z);
    }

    void set_mat4fv(const char* name, const float* mat){
        int mat_ID = glGetUniformLocation(shader_ID, name);
        glUniformMatrix4fv(mat_ID, 1, GL_FALSE, mat);
    }

public:
    std::string info(){
        std::string ret;
        // ret += "\tvshader path: " + vshader_path + "\n";
        // ret += "\tfshader path: " + fshader_path + "\n";
        ret += "\tvshader ID(" + std::to_string(vshader_ID) + ")";
        ret += "\tfshader ID(" + std::to_string(fshader_ID) + ")\n";
        ret += "\tshader ID(" + std::to_string(shader_ID) + ").\n";
        return ret;
    };
};


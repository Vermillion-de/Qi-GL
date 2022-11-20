#pragma once
#define DEV

#include <vector>
#include <string>
#include <functional>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glad.h"
#include <GL/gl.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"

// a general workflow: set -> load -> use

class Object
{
public:
    Object(){};
    Object(std::string name_) : name(name_){};
    ~Object(){};
public:
    std::string name;
    Texture texture;
    Shader shader;
public:     
    void set_object(std::string object_path, bool centerlize);
    void set_texture(std::string texture_path);
    void set_shader(std::string vs_path, std::string fs_path);

    void load();
    void render();
public:     // elementary data for render
    bool centerlize=true;
    std::string object_path;
    std::vector<glm::vec3>  v, vn;
    std::vector<glm::ivec3> f, fn; 
    std::vector<glm::vec2>  vt;
    std::vector<glm::ivec3> ft; 

    glm::vec3 center{0,0,0};
    float max = 1;
    
    void load_object();
    void bind();
    unsigned int vao_ID, vbo_ID;
    unsigned int ebo_ID;
    std::string info();
public:     // basic transformation
    float scale = 1;
    glm::vec3 delta{0,0,0};
    void scale_by(double r_){ scale = r_*scale; } // setting for transforms
    void move(float x, float y, float z){ delta += glm::vec3{x, y, z};}

public: 
    bool skeletal = true;
};

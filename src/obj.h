#pragma once
#define DEV

#include <vector>
#include <string>
#include <functional>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glad/glad.h"
#include <GL/gl.h>
#include <glm/glm.hpp>

// Notice that stb_image.h has different usage when complied into library...
#ifndef STB_IMAGE_STATIC
#ifndef STB_IMAGE_IMPLEMENTATION 
#define STB_IMAGE_IMPLEMENTATION 
#define STB_IMAGE_STATIC
#include "stb_image.h"
#endif
#endif


class Obj
{
public:
    Obj(){};
    Obj(std::string name_) : name(name_){};
    ~Obj(){};
public:     // gl is a state machine, doing this would have less bug...
    std::string name;

    bool centerlize=true;
    std::string obj_path;
    
    bool use_texture=false;
    std::string texture_path;

    bool use_shader=false;
    std::string vshader_path;
    std::string fshader_path;

public:     // elementary data for render
    std::vector<glm::vec3>  v, vn;
    std::vector<glm::ivec3> f, fn; 
    std::vector<glm::vec2>  vt;
    std::vector<glm::ivec3> ft; 
    GLuint texture_ID=0;
public:     // basic transformation
    float scale = 1;
    glm::vec3 delta{0,0,0};
    std::function<glm::vec3(glm::vec3)> transform = [&](glm::vec3 x){
        return x*scale + delta;
    };

    void scale_by(double r_){ scale = r_*scale; } // setting for transforms
    void move(float x, float y, float z){ delta += glm::vec3{x, y, z};}
public:     // for custom set
    void set_obj(std::string obj_path_, bool centerlize_) { obj_path=obj_path_; centerlize=centerlize_;};
    void set_texture(std::string texture_path_) { use_texture=true; texture_path=texture_path_;};
    void set_shader(std::string vshader_path_, std::string fshader_path_) { use_shader=true; vshader_path=vshader_path_, fshader_path=fshader_path_; };
public:     // for init
    void init_obj();
    void init_texture();
    void init_shader();
public: // settings for core render
    unsigned int vshader_ID, fshader_ID;
    unsigned int shader_ID;
    unsigned int vao_ID, vbo_ID;
    unsigned int ebo_ID;
    void core_bind();
    void core_render();
public: // settings for legacy render
    bool skeletal = true;
    void legacy_render();
public:     // for run time usages
    void texture_settings();
    void init();
    void render();

    int time=0;
};

#pragma once
#define DEV

#include "data.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GL/gl.h>

#include <vector>
#include <string>
#include <functional>
#include <fstream>

class Obj
{
private:
    
    std::string name;

    // geometry
    std::vector<vec3d> v;
    std::vector<vec3d> vn;
    std::vector<vec3i> f;    
    std::vector<vec3i> fn;
    
    // texture
    std::vector<vec2d> vt;
    std::vector<vec3i> ft;
    bool use_texture = false;
    int t_w, t_h, t_channel;
    unsigned char *pixels = NULL;
    GLuint texture_ID;
    std::string texture_path;

    // transformation for visualization
    double scale = 1;
    vec3d delta{0,0,0};
    std::function<vec3d(vec3d)> transform = [&](vec3d x){return x*scale + delta;};
    
    // visualization effect
    bool skeletal = true;
public:
    Obj(){};
    Obj(std::string name_) : name(name_){};
    Obj(std::string obj_path,std::string name_, bool centerlize);
    Obj(std::string obj_path, std::string texture_path, std::string name_, bool centerlize);
    ~Obj(){ if(pixels != NULL) stbi_image_free(pixels); };
public: 

    void clear_obj_data();
    void load_obj(std::string obj_path, bool centerlize);
    void load_texture(std::string texture_path);
    void setup_texture();
    void init(){ };

public:

    void scale_by(double r_){
        scale = r_*scale;
#ifndef DEV
        scale = std::min(scale,100.);
        scale = std::max(scale,0.01);
#endif
    }
    void move(vec3d d){ delta = delta + d; }
    void render();
};

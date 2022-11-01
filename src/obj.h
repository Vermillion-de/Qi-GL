#pragma once
#define DEV

#include <vector>
#include <string>
#include <functional>
#include <fstream>
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

#include "data.h"

class Obj
{
public:
    std::string name;
public: // settings for legacy render
    // geometry
    std::vector<glm::vec3>  v;
    std::vector<glm::ivec3> f;  
    std::vector<glm::vec3>  vn;  
    std::vector<glm::ivec3> fn;
    
    // texture
    std::vector<glm::vec2>  vt;
    std::vector<glm::ivec3> ft;
    bool use_texture = false;
    int t_w, t_h, t_channel;
    unsigned char *pixels = NULL;
    GLuint texture_ID=-1;
    std::string texture_path;

    // transformation for visualization
    double scale = 1;
    glm::vec3 delta{0,0,0};
    double theta = 0, phi = 0;
    std::function<glm::vec3(glm::vec3)> transform = [&](glm::vec3 x){ x *= scale; return x + delta; };
    void scale_by(double r_){ scale = r_*scale; } // setting for transforms
    void move(glm::vec3 d){ delta += d; }
    void move(vec3d d){ delta += glm::vec3{d.x, d.y, d.z};}
    void move(float x, float y, float z){ delta += glm::vec3{x, y, z};}
    
    // visualization effect
    bool skeletal = true;

    void legacy_render();
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
public: // settings for core render
    void core_bind();
    void core_render();
    
    void render();
};

#pragma once
#include <glad.h>
#include <GL/gl.h>

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>


// a setting for stb_image for static library compiling
#ifndef STB_IMAGE_STATIC
#ifndef STB_IMAGE_IMPLEMENTATION 
#define STB_IMAGE_IMPLEMENTATION 
#define STB_IMAGE_STATIC
#include "stb_image.h"
#endif
#endif

class Texture
{
private:
    std::string path;
    unsigned int texture_ID;
    std::string info;
public:
    Texture(std::string texture_path):path(texture_path){};
    ~Texture(){};
public:
    void load(){
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        int t_w, t_h, t_channel;
        unsigned char *pixels = stbi_load(path.c_str(), &t_w, &t_h, &t_channel, 0); 
        if (pixels==NULL) { std::cout << " load texture " << path  <<  " failed!"; return; }
        glGenTextures(1, &texture_ID);
        glBindTexture(GL_TEXTURE_2D, texture_ID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_w, t_h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        stbi_image_free(pixels);

        info += "\tFrom \"" +  path + "\n";
        info += "\tSize : (h,w,c) = (" + std::to_string(t_h) + ", " + std::to_string(t_w) + "," + std::to_string(t_channel) + "). to texture_ID: " + std::to_string(texture_ID) + ".\n";
    }

    void bind(){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_ID);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 线形滤波
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 线形滤波
    }

    std::string information(){
        return info;
    }
};


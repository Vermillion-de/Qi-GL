#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>

#include <glad.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

#include "shader.h"
#include "texture.h"
#include "vertices.h"

class GL
{
private:
    // std::vector<int>        vaoIDs;
    // std::vector<int>        vboIDs;
    // std::vector<int>        veoIDs;
    std::vector<Texture>    textures;
    std::vector<Shader>     shaders;
    std::vector<Vertices>   vertices;
public:
    int init(std::map<std::string, std::string> config){
        textures.push_back({config["texture_path"]});
        shaders.push_back({config["vshader_path"], config["fshader_path"]});
        // vertices.push_back({});
    }
    int load(){
        textures.back().load();
        shaders.back().load();
    }
    int render(){
        textures.back().bind();
        shaders.back().bind();
        if (vertices.size() != 0)
        {
            vertices.back().bind();
        }
    }
public:
    int add_obj(){
        
    }

public:
    GL(){};
    GL(std::map<std::string, std::string> config){

    };
    ~GL();
};

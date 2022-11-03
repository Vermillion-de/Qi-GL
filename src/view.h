#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <math.h>

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <glm/gtc/matrix_transform.hpp>

#include "./imgui/imgui_glfw.h"
#include "obj.h"

class View
{
public: // window situation
    GLFWwindow* window=NULL;   
    int windowWidth=1200, windowHeight=1200;   
    // mouse condition last time
    std::pair<glm::ivec2, bool> mouseLeft{{0,0},false};
    std::pair<glm::ivec2, bool> mouseMid{{0,0},false};
    std::pair<glm::ivec2, bool> mouseRight{{0,0},false};
    double mouse_x=0, mouse_y=0;
public: //glfw settings
    void glfw_init();
public: // imgui things
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    void imgui_init();
    void imgui_render(); 
    void imgui_end(); 
public:     // static things for callbacks...
    static View* global_view;   
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_click_callback(GLFWwindow* window, int button, int action, int mods);
    static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
public:     // camera, sence things
    float r = 0.1;
    int theta = 0, phi = 135;
    float dx=3, dy=3, dz=3;     // for orth view size

    bool use_light = false;
    bool show_cube = true;
    bool show_axis = true;
private:    // renderred data
    Obj data;
    char data_path[100];
public:  
    View(){};
    ~View(){};
public:     // react of this functions
    void keyReact(unsigned char key);
    void mouseClickReact(int button, int state);
    void mouseScrollReact( double xoffset, double yoffset);
    void reshapeWindowReact();
    void moveMouseReact();
    // void reshapeWindowReact(int w, int h);  
    // void load(std::string obj_path, bool centerlize);
    void load(std::string obj_path, bool centerlize);
    void load(std::string obj_path, std::string texture_path, bool centerlize);
    void load(std::string obj_path, std::string vshader_path, std::string fshader_path, bool centerlize);
    void load(std::string obj_path, std::string texture_path, std::string vshader_path, std::string fshader_path, bool centerlize);
    void render();
    void show(int argc, char **argv);
};

#ifndef USE_QIView
View* View::global_view = nullptr;
#endif
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <math.h>

#include <GLFW/glfw3.h>
#include <GL/glu.h>

#include "../imgui/imgui_glfw.h"
#include "data.h"
#include "obj.cpp"

class View
{
public: // window situation
    GLFWwindow* window=NULL;   
    int windowWidth=1200, windowHeight=1200;   
    // mouse condition last time
    std::pair<vec2i, bool> mouseLeft{{0,0},false};
    std::pair<vec2i, bool> mouseMid{{0,0},false};
    std::pair<vec2i, bool> mouseRight{{0,0},false};
public: //glfw settings
    void glfw_init();
public: // imgui things
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    void imgui_init();
    void imgui_render(); 
    void imgui_end(); 
public:     // static things for callbacks...
    static View* global_view;   
    static void reshapeWindow(int w, int h);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_click_callback(GLFWwindow* window, int button, int action, int mods);
    static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
public:     // camera, sence things
    double r = sqrt(3);
    int theta = 0, phi = 135;
    bool use_light = false;
    float dx=3, dy=3, dz=3;     // for orth view size
private:    // renderred data
    Obj data;
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
    void load(std::string obj_path, std::string texture_path,bool centerlize);
    void render();
    void show(int argc, char **argv);
};
View* View::global_view = nullptr;

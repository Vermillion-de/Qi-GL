#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>

#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <math.h>

#include "data.h"
#include "obj.cpp"

class View
{
public:
    GLFWwindow* window=NULL;     
    // camera on a sphere
    double r = sqrt(3);
    int theta = 0, phi = 135;
    // mouse condition last time
    std::pair<vec2i, bool> mouseLeft{{0,0},false};
    std::pair<vec2i, bool> mouseMid{{0,0},false};
    std::pair<vec2i, bool> mouseRight{{0,0},false};
    // window situation
    int windowWidth = 1200, windowHeight = 1200; 
public:
    bool use_light = false;
private:    // data
    Obj data;
public:  
    View(){};
    ~View(){};
public:     // static things to setup callbacks..
    static View* global_view;   
    static void reshapeWindow(int w, int h);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_click_callback(GLFWwindow* window, int button, int action, int mods);
    static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
public:     // this functions
    void keyReact(unsigned char key);
    void mouseClickReact(int button, int state);
    void mouseScrollReact( double xoffset, double yoffset);

    void moveMouseReact();
    // void reshapeWindowReact(int w, int h);
    
    // void load(std::string obj_path, bool centerlize);
    void load(std::string obj_path, std::string texture_path,bool centerlize);
    void render();
    void show(int argc, char **argv);
};

View* View::global_view = nullptr;

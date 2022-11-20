#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <map>
#include <math.h>

#include <glad.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui_glfw.h>
#include "object.h"

class View
{
public:  
    View(){};
    ~View(){};
public:     // static things for callbacks...
    static View* global_view;   
    static void key_callback(GLFWwindow*, int, int, int, int);
    static void mouse_click_callback(GLFWwindow*, int, int, int);
    static void mouse_scroll_callback(GLFWwindow*, double, double);

public: // window situation
    GLFWwindow* window=NULL;   
    int windowWidth=1800, windowHeight=1500;   
    // mouse condition last time
    std::pair<glm::ivec2, bool> mouseLeft{{0,0},false};
    std::pair<glm::ivec2, bool> mouseMid{{0,0},false};
    std::pair<glm::ivec2, bool> mouseRight{{0,0},false};
    double mouse_x=0, mouse_y=0;

    virtual void keyReact(unsigned char key);
    virtual void mouseClickReact(int button, int state);
    virtual void mouseScrollReact( double xoffset, double yoffset);
    virtual void reshapeWindowReact();
    virtual void moveMouseReact();

public: //glfw and ImGui settings
    void glfw_init();
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.00f, 1.00f);
    void imgui_init();
    void imgui_render(); 
    void imgui_end(); 
public:     // camera, sence things
    float r = 0.1;
    int theta = 0, phi = 135;
    float dx=3, dy=3, dz=3;     // for orth view size
    
    bool show_cube = false;
    bool show_axis = false;

    bool show_simulate = false;
    float l0 = 1;
    float k = 100;

private:    // renderred data
    Object data;
    Object data_backup;
public:     // react of this functions
    void load(std::map<std::string, std::string> config);
    void render();
    void show(int argc, char **argv);
};

#ifndef USE_QIView
View* View::global_view = nullptr;
#endif
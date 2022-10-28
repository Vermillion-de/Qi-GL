#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>

#include <GL/glut.h>
#include <math.h>

#include "data.h"
#include "obj.cpp"

class View
{
public:     
    // camer at a sphere
    double r = sqrt(3);
    int theta = 0, phi = 135;
    // mouse condition last time
    std::pair<vec2i, bool> mouseLeft{{0,0},false};
    std::pair<vec2i, bool> mouseMid{{0,0},false};
    // window situation
    int windowWidth = 1200, windowHeight = 1200; 
public:
    bool use_light = false;
private:    // data
    Obj data;
public:  
    View(){};
    ~View(){};
public:     // static things
    static View* global_view;   // a global example, for rendering this.
    static void renderScene();
    static void reshapeWindow(int w, int h);
    static void pressKey(unsigned char key, int x, int y);
    static void clickMouse(int button, int state, int x, int y);
    static void moveMouse(int x, int y);
public:     // this functions
    void load(std::string objname, std::string texturename,bool centerlized);
    void sceneReact();
    void keyReact(unsigned char key, int x, int y);
    void mouseClickReact(int button, int state, int x, int y);
    void moveMouseReact(int x, int y);
    void reshapeWindowReact(int w, int h);
    void show(int argc, char **argv);
};
View* View::global_view = nullptr;

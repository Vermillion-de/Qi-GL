#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>

#include <GL/glut.h>
#include <math.h>

#include "data.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define DEV

class View
{
public:     // need for camer parameters
    double r = sqrt(3);
    double scale = 1;
    vec3d delta{0,0,0};
    std::function<vec3d(vec3d)> transform = [&](vec3d x){return x*scale + delta;};

    double dx = 0, dy = 0, dz = 0;
    int theta = 0, phi = 135;

    std::pair<vec2i, bool> mouseLeft{{0,0},false};
    std::pair<vec2i, bool> mouseMid{{0,0},false};

    int windowWidth = 1600, windowHeight = 1600; 

    bool use_light = false;
    bool use_texture = true;
private:    // data
    std::vector<vec3d> v;
    std::vector<vec2d> vt;
    std::vector<vec3d> vn;

    std::vector<vec3i> f;    
    std::vector<vec3i> ft;
    std::vector<vec3i> fn;

    std::vector<vec3d> c;
public:  
    View(){};
    View(std::vector<vec3d> v_,std::vector<vec3i>f_):v(v_), f(f_){ };
    ~View(){};
public:     // static things
    static View* global_view;   // a global example, for rendering this.
    static void renderScene();
    static void reshapeWindow(int w, int h);
    static void pressKey(unsigned char key, int x, int y);
    static void clickMouse(int button, int state, int x, int y);
    static void moveMouse(int x, int y);

public:     // this functions

    void load(std::vector<vec3d> v_, std::vector<vec3i> f_);
    void load(std::function<double(int,int)> fv, int nv, std::function<int(int,int)> ff, int nf); // generally works for most case.
    void load(std::string objname, std::string texturename,bool centerlized);

    void load_texture(std::string img_path);

    void sceneReact();
    void keyReact(unsigned char key, int x, int y);
    void mouseClickReact(int button, int state, int x, int y);
    void moveMouseReact(int x, int y);
    void reshapeWindowReact(int w, int h);

    void show(int argc, char **argv);
};
View* View::global_view = nullptr;

//////// some useful functions
void drawCube(double scalar){
    glBegin(GL_LINES);
        glColor3f(0.5,0.5,0.5);
        
        glVertex3d(-scalar,-scalar,-scalar);
        glVertex3d(-scalar, scalar,-scalar);
        glVertex3d(-scalar, scalar,-scalar);
        glVertex3d( scalar, scalar,-scalar);
        glVertex3d( scalar, scalar,-scalar);
        glVertex3d( scalar,-scalar,-scalar);
        glVertex3d( scalar,-scalar,-scalar);
        glVertex3d(-scalar,-scalar,-scalar);

        glVertex3d(-scalar,-scalar,scalar);
        glVertex3d(-scalar, scalar,scalar);  
        glVertex3d(-scalar, scalar,scalar);
        glVertex3d( scalar, scalar,scalar);
        glVertex3d( scalar, scalar,scalar);
        glVertex3d( scalar,-scalar,scalar);
        glVertex3d( scalar,-scalar,scalar);
        glVertex3d(-scalar,-scalar,scalar);

        glVertex3d(-scalar,-scalar,-scalar);
        glVertex3d(-scalar,-scalar, scalar);  
        glVertex3d(-scalar, scalar,-scalar);
        glVertex3d(-scalar, scalar, scalar);
        glVertex3d( scalar, scalar,-scalar);
        glVertex3d( scalar, scalar, scalar);
        glVertex3d( scalar,-scalar,-scalar);
        glVertex3d( scalar,-scalar, scalar);

    glEnd();
}

void drawAxis(double scalar){
    glBegin(GL_LINES);
        glColor3f(1,0,0); // x <-> red
        glVertex3f(-scalar, 0, 0);
        glVertex3f( scalar, 0, 0);

        glColor3f(0,1,0); // y <-> green
        glVertex3f(0,-scalar,0);
        glVertex3f(0, scalar,0);

        glColor3f(0,0,1); // z <-> blue
        glVertex3f(0,0,-scalar);
        glVertex3f(0,0, scalar);
    glEnd();
}
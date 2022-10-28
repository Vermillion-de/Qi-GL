#pragma once
#include <GL/gl.h>

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
#pragma once
#include <GL/gl.h>

//////// some useful functions
void drawCube(double scalar){
    glBegin(GL_LINES);
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
        glVertex3f(-scalar, 0, 0);
        glVertex3f( scalar, 0, 0);

        glVertex3f(0,-scalar,0);
        glVertex3f(0, scalar,0);

        glVertex3f(0,0,-scalar);
        glVertex3f(0,0, scalar);
    glEnd();
}
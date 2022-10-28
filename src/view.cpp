#include "view.h"
#include "misc.h"

void View::renderScene(){  // render a scene
    
    glClearColor(0.1, 0.1, 0.1, 0.2);   // nice background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    View::global_view->sceneReact();

    glutSwapBuffers();
}

void View::reshapeWindow(int w, int h) {    // recall of window.reshapeWindow
#ifdef DEV
    std::cout << "Resize window into (" << w << "," << h << ")." << std::endl; // TODO
#endif
    View::global_view->reshapeWindowReact(w,h);
}

void View::pressKey(unsigned char key, int x, int y){
#ifdef DEV
    std::cout << "Press key (" << key << ") as pos(" << x << "," << y << ")" << std::endl; 
    if (key == 27)  //esc
    {
        std::cout << "Quit.." << std::endl; 
        exit(0);
    }
#endif
    View::global_view->keyReact(key,x,y);
}

void View::clickMouse(int button, int state, int x, int y){
#ifdef DEV
    std::cout << "Press mouse (" << button << ")" << " as (" << state << ") at (" << x << "," << y << ")." << std::endl; 
#endif
    View::global_view->mouseClickReact(button, state, x, y);    
}

void View::moveMouse(int x, int y){
#ifdef DEV
    std::cout << "Move mouse (" << x << "," << y << ")." << std::endl;
#endif
    View::global_view->moveMouseReact(x,y);

}

void View::load(std::string obj_path, std::string texture_path, bool centerlize=true){    // TODO: aware of the size...
   data.load_obj(obj_path, centerlize);
   data.load_texture(texture_path);
}

void View::sceneReact(){

    //           eye position  look at point   head up 
    // gluLookAt(0.0,0.0,1.0,  0.0,0.0,-1.0,   0.0,1.0,0.0);
    // gluLookAt(0.0,1.0,0.0, 0.0,0.0,0.0, 0.0,0.0,-1.0);
        
    glLoadIdentity();
    glOrtho(-3,3,-3,3,-3,3);

    double Theta = theta/180.*M_PI, Phi = phi/180.*M_PI;
    double h = -r*cos(Phi), l = r*sin(Phi);
    gluLookAt(l*cos(Theta),h,l*sin(Theta),  0.0,0.0,0.0,  -h*cos(Theta),l,-h*sin(Theta));
    
    
    if (use_light)
    {
        glEnable(GL_LIGHTING);
	    glEnable(GL_LIGHT0);
	    GLfloat ambient[] = { .3f,.3f,.3f,1.f };
	    GLfloat diffuse[] = { .7f,.7f,.7f,1.f };
	    GLfloat specular[] = { .5f,.5f,.5f,1.f };
	    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	    GLfloat position[] = { 1.0f,0.0f,0.f,1.f };
	    glLightfv(GL_LIGHT0, GL_POSITION, position);
    }
    data.render();
#ifdef DEV
    drawCube(1);
    drawAxis(1);
#endif
}

void View::mouseClickReact(int button, int state, int x, int y){
    enum{ left = 0, mid = 1, right = 2, scroll_up = 3, scroll_down = 4 };
    enum{ press = 0, release = 1};
    switch (button)
    {
    case left :
        if (state == press && mouseLeft.second == false)  
            mouseLeft = std::pair<vec2i, bool>{{x,y},true};
        else if (state == release)
            mouseLeft = std::pair<vec2i, bool>{{0,0},false};
        break;
    case mid :
        if (state == press && mouseMid.second == false)  
            mouseMid = std::pair<vec2i, bool>{{x,y},true};
        else if (state == release)
            mouseMid = std::pair<vec2i, bool>{{0,0},false};
        break;
    case scroll_up:
        data.scale_by(1.05);
        glutPostRedisplay();
        break;
    case scroll_down:
        data.scale_by(0.95);
        glutPostRedisplay();
        break;
    default:
        break;
    }
}

void View::moveMouseReact(int x, int y){
    if (mouseLeft.second == true){
        theta += (x - mouseLeft.first.x)/2;
        phi += (y - mouseLeft.first.y)/2;
        theta %= 360; phi %= 360;
        mouseLeft = std::pair<vec2i, bool>{{x,y},true};
        glutPostRedisplay();
    }
    else if (mouseMid.second == true){
        double Theta = theta/180.*M_PI, Phi = phi/180.*M_PI;
        double dx = (x - mouseMid.first.x);
        double dy = (y - mouseMid.first.y);
        data.move( vec3d{sin(Phi)*sin(Theta),0,-sin(Phi)*cos(Theta)}*dx/200. + vec3d{-cos(Phi)*cos(Theta),-sin(Phi),-cos(Phi)*sin(Theta)}*dy/200.);
        mouseMid = std::pair<vec2i, bool>{{x,y},true};
        glutPostRedisplay();
    }
}

void View::keyReact(unsigned char key, int x, int y){
    double Theta = theta/180.*M_PI, Phi = phi/180.*M_PI;
    switch (key)
    {
    // rotate object
    case 'h': case 'a':
        theta--; theta %= 360;
        glutPostRedisplay();
        break;
    case 'l': case 'd':
        theta++; theta %= 360;
        glutPostRedisplay();
        break;
    case 'k': case 'w':
        phi--; phi %= 360;
        glutPostRedisplay();
        break;
    case 'j': case 's':
        phi++; phi %= 360;
        glutPostRedisplay();
        break;
    // move object
    case 'H': case 'A':
        data.move(vec3d{-sin(Phi)*sin(Theta),0,sin(Phi)*cos(Theta)}/50.);
        glutPostRedisplay();
        break;
    case 'L': case 'D':
        data.move(vec3d{sin(Phi)*sin(Theta),0,-sin(Phi)*cos(Theta)}/50.);    
        glutPostRedisplay();
        break;
    case 'K': case 'W':
        data.move(vec3d{cos(Phi)*cos(Theta),sin(Phi),cos(Phi)*sin(Theta)}/50.);
        glutPostRedisplay();
        break;
    case 'J': case 'S':
        data.move( vec3d{-cos(Phi)*cos(Theta),-sin(Phi),-cos(Phi)*sin(Theta)}/50.);
        glutPostRedisplay();
        break;
    case 'm':
        r = std::max(0.01,r-0.05);
        glutPostRedisplay();
        break;
    case 'M':
        r = std::min(100.,r+0.05);
        glutPostRedisplay();
        break;
    default:
        break;
    }
}

void View::reshapeWindowReact(int w, int h){

    glViewport(0,0,w,h);

}

void View::show(int argc, char **argv){
    // set the instance this into global_view
    global_view = this;

	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(windowWidth,windowHeight);
	glutCreateWindow("View");

	glutDisplayFunc(renderScene);       // sceneReact scene

    glutReshapeFunc(reshapeWindow);     // recall for window resizing.
    glutKeyboardFunc(pressKey);         // recall for press keyboard
    glutMouseFunc(clickMouse);          // recall for click mouse
    glutMotionFunc(moveMouse);          //

    // glutIdleFunc(renderScene);       // run display every possible

    glutMainLoop();                     // enter GLUT event processing cycle
}
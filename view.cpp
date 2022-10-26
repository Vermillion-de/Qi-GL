#include "view.h"

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

void View::load(std::vector<vec3d> v_, std::vector<vec3i> f_){
    v = v_;
    f = f_;
}

void View::load(std::function<double(int,int)> fv, int nv, std::function<int(int,int)> ff, int nf){
    v.resize(nv), f.resize(nf);
    for (int i = 0; i < nv; i++)
    {
        v[i] = {fv(i,0), fv(i,1), fv(i,2)};
    }
    
    for (int i = 0; i < nf; i++)
    {
        f[i] = {ff(i,0), ff(i,1), ff(i,2)};
    }
}

void View::load(std::string filename, bool centerlized=true){    // TODO: aware of the size...
    std::ifstream file(filename);
    char type;
    double  x, y, z;
    v.clear(), f.clear();
    v.push_back({0,0,0});
    while (file >> type >> x >> y >> z)
    {   
        if (type == 'f') {f.push_back({x,y,z});}
        if (type == 'v') {v.push_back({x,y,z});}
    }
    
    file.close();
    if (centerlized)
    {
        vec3d center{0,0,0};
        for (vec3d& v_ : v) center = center + v_; 
        center = center / v.size();

        double max = 0;
        for (vec3d& v_ : v)
        {
            double temp = (v_-center).max();
            max = (max > temp ? max : temp);
        }
        for (vec3d& v_ : v) { v_ = (v_-center)/max;}
#ifdef DEV
    std::cout << "v size : " << v.size() << std::endl;
    std::cout << "f size : " << f.size() << std::endl;
    std::cout << "Center :" ;
    center.print();
    std::cout << std::endl;
    std::cout << "max :" << max << std::endl;
#endif

    }

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
    
    // gluLookAt(0,1,0,0,0,0,1,0,0);
    // gluOrtho2D();
    // gluPerspective(170, 1, 0.1, 100);

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

    glColor3f(1.,1.,1.);
    glBegin(GL_TRIANGLES);
        for (vec3i& fi : f)
        {
            vec3d fa = transform(v[fi.a]);
            vec3d fb = transform(v[fi.b]);
            vec3d fc = transform(v[fi.c]);
            glVertex3f(fa.x, fa.y, fa.z);
            glVertex3f(fb.x, fb.y, fb.z);
            glVertex3f(fc.x, fc.y, fc.z);
        }
    glEnd();

    glColor3f(0,0,0);

    // if(f.size() < 2000) 
    glLineWidth(2);
    glBegin(GL_LINES);
	    for (vec3i& fi : f)
        {
            vec3d fa = transform(v[fi.a]);
            vec3d fb = transform(v[fi.b]);
            vec3d fc = transform(v[fi.c]);
            glVertex3f(fa.x, fa.y, fa.z);
            glVertex3f(fb.x, fb.y, fb.z);
            glVertex3f(fb.x, fb.y, fb.z);
            glVertex3f(fc.x, fc.y, fc.z);
            glVertex3f(fc.x, fc.y, fc.z);
            glVertex3f(fa.x, fa.y, fa.z);
        }
    glEnd(); 

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
        scale = std::min(scale*1.05,100.);
        glutPostRedisplay();
        break;
    case scroll_down:
        scale = std::max(scale*0.95,0.01);
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
        delta = delta + vec3d{sin(Phi)*sin(Theta),0,-sin(Phi)*cos(Theta)}*dx/200. + vec3d{-cos(Phi)*cos(Theta),-sin(Phi),-cos(Phi)*sin(Theta)}*dy/200.;
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
        delta = delta - vec3d{sin(Phi)*sin(Theta),0,-sin(Phi)*cos(Theta)}/50.;
        glutPostRedisplay();
        break;
    case 'L': case 'D':
        delta = delta + vec3d{sin(Phi)*sin(Theta),0,-sin(Phi)*cos(Theta)}/50.;
        glutPostRedisplay();
        break;
    case 'K': case 'W':
        delta = delta - vec3d{-cos(Phi)*cos(Theta),-sin(Phi),-cos(Phi)*sin(Theta)}/50.;
        glutPostRedisplay();
        break;
    case 'J': case 'S':
        delta = delta + vec3d{-cos(Phi)*cos(Theta),-sin(Phi),-cos(Phi)*sin(Theta)}/50.;
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

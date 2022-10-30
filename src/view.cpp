#include "view.h"
#include "misc.h"

void View::reshapeWindow(int w, int h) {    // recall of window.reshapeWindow
#ifdef DEV
    std::cout << "Resize window into (" << w << "," << h << ")." << std::endl; // TODO
#endif
    View::global_view->reshapeWindowReact();
}

void View::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
#ifdef DEV
    std::cout << "Pressed key (" << (char)key << ") as (" <<  scancode << ") with action, " << action << " in mode " << mods << "." << std::endl; 
    if (key == GLFW_KEY_ESCAPE) { std::cout << "Pressed key [esc], so quit.." << std::endl;  exit(0); }
#endif
    View::global_view->keyReact(key);
}

void View::mouse_click_callback(GLFWwindow* window, int button, int action, int mods){
#ifdef DEV
    std::cout << "Pressed mouse (" << button << ")" << " as (" << action << ")." << std::endl; 
#endif
    View::global_view->mouseClickReact(button, action);    
}

void View::mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
#ifdef DEV
    std::cout << "Scrolled mouse (" << xoffset << "," << yoffset << ")." << std::endl; 
#endif
    View::global_view->mouseScrollReact(xoffset,yoffset);
}

void View::reshapeWindowReact(){
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    if (windowWidth == windowHeight)        {glViewport(0,0,windowWidth, windowHeight);}
    else if (windowWidth > windowHeight)    {glViewport((windowWidth-windowHeight)/2,0,windowHeight, windowHeight);}
    else if (windowWidth < windowHeight)    {glViewport(0,(windowHeight-windowWidth)/2, windowWidth, windowWidth);}    
}

void View::mouseClickReact(int button, int action){
    double x,y;
    glfwGetCursorPos(window,&x,&y);
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT :
        // std::cout << "Pressed mouse left!" << std::endl;
        if (action == GLFW_PRESS && mouseLeft.second == false)  
            mouseLeft = std::pair<vec2i, bool>{{x,y},true};
        else if (action == GLFW_RELEASE)
            mouseLeft = std::pair<vec2i, bool>{{0,0},false};
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        // std::cout << "Pressed mouse mid!" << std::endl;
        if (action == GLFW_PRESS && mouseMid.second == false)  
            mouseMid = std::pair<vec2i, bool>{{x,y},true};
        else if (action == GLFW_RELEASE)
            mouseMid = std::pair<vec2i, bool>{{0,0},false};
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        // std::cout << "Pressed mouse Right!" << std::endl;
        if (action == GLFW_PRESS && mouseMid.second == false)  
            mouseMid = std::pair<vec2i, bool>{{x,y},true};
        else if (action == GLFW_RELEASE)
            mouseMid = std::pair<vec2i, bool>{{0,0},false};
        break;
    default: break;
    }
}

void View::mouseScrollReact( double xoffset, double yoffset){
    theta += xoffset*2;
    data.scale_by(1.+yoffset*0.05);
}

void View::moveMouseReact(){
    double x,y;
    glfwGetCursorPos(window, &x, &y);
// #ifdef DEV
//     std::cout << "Move mouse (" << x << "," << y << ")." << std::endl;
// #endif
    if (mouseLeft.second == true){
        theta += (x - mouseLeft.first.x)/2;
        phi += (y - mouseLeft.first.y)/2;
        theta %= 360; phi %= 360;
        mouseLeft = std::pair<vec2i, bool>{{x,y},true};
    }
    else if (mouseMid.second == true){
        double Theta = theta/180.*M_PI, Phi = phi/180.*M_PI;
        double dx = (x - mouseMid.first.x);
        double dy = (y - mouseMid.first.y);
        data.move(vec3d{sin(Phi)*sin(Theta),0,-sin(Phi)*cos(Theta)}*dx/200. + vec3d{-cos(Phi)*cos(Theta),-sin(Phi),-cos(Phi)*sin(Theta)}*dy/200.);
        mouseMid = std::pair<vec2i, bool>{{x,y},true};
    }
}

void View::keyReact(unsigned char key){
    double Theta = theta/180.*M_PI, Phi = phi/180.*M_PI;
    // double x, y;
    // glfwGetCursorPos(window, &x, &y);
    switch (key){
        // rotate object
        case 'H': theta--; theta %= 360; break;
        case 'L': theta++; theta %= 360; break;
        case 'K': phi--; phi %= 360; break;
        case 'J': phi++; phi %= 360; break;
        // move object
        case 'A': data.move(vec3d{-sin(Phi)*sin(Theta),0,sin(Phi)*cos(Theta)}/50.); break;
        case 'D': data.move(vec3d{sin(Phi)*sin(Theta),0,-sin(Phi)*cos(Theta)}/50.); break;
        case 'W': data.move(vec3d{cos(Phi)*cos(Theta),sin(Phi),cos(Phi)*sin(Theta)}/50.); break;
        case 'S': data.move(vec3d{-cos(Phi)*cos(Theta),-sin(Phi),-cos(Phi)*sin(Theta)}/50.); break;
        case 'm': r = std::max(0.01,r-0.05); break;
        case 'M': r = std::min(100.,r+0.05); break;
        default: break;
    }
}

// void View::reshapeWindowReact(int w, int h){

//     glViewport(0,0,w,h);

// }

void View::load(std::string obj_path, std::string texture_path, bool centerlize=true){    // TODO: aware of the size...
   data.load_obj(obj_path, centerlize);
   data.load_texture(texture_path);
}

void View::render(){    
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    // glClearColor(0.1, 0.1, 0.1, 0.2);   // nice background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glLoadIdentity();
    glOrtho(-dx,dx,-dy,dy,-dz,dz);

    double Theta = theta/180.*M_PI, Phi = phi/180.*M_PI;
    double h = -r*cos(Phi), l = r*sin(Phi);
    gluLookAt(l*cos(Theta),h,l*sin(Theta),  0.0,0.0,0.0,  -h*cos(Theta),l,-h*sin(Theta));
    // if (use_light)
    // {
    //     glEnable(GL_LIGHTING);
	//     glEnable(GL_LIGHT0);
	//     GLfloat ambient[] = { .3f,.3f,.3f,1.f };
	//     GLfloat diffuse[] = { .7f,.7f,.7f,1.f };
	//     GLfloat specular[] = { .5f,.5f,.5f,1.f };
	//     glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	//     glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	//     glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	//     GLfloat position[] = { 1.0f,0.0f,0.f,1.f };
	//     glLightfv(GL_LIGHT0, GL_POSITION, position);
    // }
    data.render();
#ifdef DEV
    drawCube(1);
    drawAxis(1);
#endif
}

void error_callback(int error, const char* description){
    std::cout << "Error (" << error << ") : " << description << std::endl;
}

void View::imgui_init(){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();
    io.Fonts->AddFontFromFileTTF("./imgui/DroidSans.ttf", 24.0f);
}

void View::imgui_render(){
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    static int counter = 0;
    ImGui::Begin("Camera Settings");                         
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);
    
    ImGui::SliderFloat("x orth box", &dx, 0.0f, 10.0f);
    ImGui::SliderFloat("y orth box", &dy, 0.0f, 10.0f);
    ImGui::SliderFloat("z orth box", &dz, 0.0f, 10.0f);

    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
    if (ImGui::Button("Button")) counter++; ImGui::SameLine(); ImGui::Text("counter = %d", counter);
    
    ImGui::End();
    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }
    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void View::imgui_end(){ // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void View::glfw_init(){
    glfwSetErrorCallback(error_callback);
    if(!glfwInit()) { std::cout << "can not init glfw window!" << std::endl; exit(-1);} 
    window = glfwCreateWindow(windowWidth, windowHeight, "Window", NULL, NULL);
    if(!window) {std::cout << "can not creat glfw window" << std::endl; exit(-1);}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 0);
    glfwMakeContextCurrent(window);
    // glfwSwapInterval(1);
    // glfwSetWindowCloseCallback(); // set function which act when close..
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_click_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);
}

void View::show(int argc, char **argv){
    global_view = this;

    glfw_init();
    imgui_init();
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        this->render();
        this->moveMouseReact();
        this->reshapeWindowReact();
        imgui_render();
        reshapeWindowReact();
        glfwSwapBuffers(window);
    }
    imgui_end();
    glfwDestroyWindow(window);
    glfwTerminate();
}
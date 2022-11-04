#include "view.h"
#include "misc.h"

void View::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
#ifdef DEV
    std::cout << "Pressed key (" << (char)key << ") as (" <<  scancode << ") with action, " << action << " in mode " << mods << "." << std::endl; 
#endif
    if (key == GLFW_KEY_ESCAPE) { std::cout << "Pressed key [esc], so quit.." << std::endl;  exit(0); }
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
    if(ImGui::GetIO().WantCaptureMouse){return;}    // mouse is on ImGUI
    glfwGetCursorPos(window,&mouse_x,&mouse_y);
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT :
        // std::cout << "Pressed mouse left!" << std::endl;
        if (action == GLFW_PRESS && mouseLeft.second == false)  
            mouseLeft = std::pair<glm::ivec2, bool>{{mouse_x,mouse_y},true};
        else if (action == GLFW_RELEASE)
            mouseLeft = std::pair<glm::ivec2, bool>{{0,0},false};
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        // std::cout << "Pressed mouse mid!" << std::endl;
        if (action == GLFW_PRESS && mouseMid.second == false)  
            mouseMid = std::pair<glm::ivec2, bool>{{mouse_x,mouse_y},true};
        else if (action == GLFW_RELEASE)
            mouseMid = std::pair<glm::ivec2, bool>{{0,0},false};
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        std::cout << "Pressed mouse Right!" << std::endl;
    default: break;
    }
}

void View::mouseScrollReact( double xoffset, double yoffset){
    if(ImGui::GetIO().WantCaptureMouse){ return; }   // mouse is on ImGUI
    theta += xoffset*2;
    data.scale_by(1.+yoffset*0.05);
}

void View::moveMouseReact(){
    if(ImGui::GetIO().WantCaptureMouse){ return; }   // mouse is on ImGUI
    glfwGetCursorPos(window, &mouse_x, &mouse_y);   
    if (mouseLeft.second == true){
        theta -= (mouse_x - mouseLeft.first.x)/2;
        phi -= (mouse_y - mouseLeft.first.y)/2;
        theta %= 360; phi %= 360;
        mouseLeft =  std::pair<glm::ivec2, bool>{{mouse_x, mouse_y}, true};
    }
    else if (mouseMid.second == true){
        double Theta = theta/180.*M_PI, Phi = phi/180.*M_PI;
        double dx = (mouse_x - mouseMid.first.x);
        double dy = (mouse_y - mouseMid.first.y);
        data.move( sin(Phi)*sin(Theta)*dx/200.,     0,                  -sin(Phi)*cos(Theta)*dx/200.);
        data.move(-cos(Phi)*cos(Theta)*dy/200.,    -sin(Phi)*dy/200.,   -cos(Phi)*sin(Theta)*dy/200.);
        mouseMid =  std::pair<glm::ivec2, bool>{{mouse_x, mouse_y}, true};
    }
}

void View::keyReact(unsigned char key){
    if(ImGui::GetIO().WantCaptureKeyboard){ return;}   // key on ImGUI
    double Theta = theta/180.*M_PI, Phi = phi/180.*M_PI;
    switch (key){
        // rotate object
        case 'H': theta++; theta %= 360; break;
        case 'L': theta--; theta %= 360; break;
        case 'K': phi++; phi %= 360; break;
        case 'J': phi--; phi %= 360; break;
        // move object
        case 'A': data.move(-sin(Phi)*sin(Theta)/50.,   0,              sin(Phi)*cos(Theta)/50.); break;
        case 'D': data.move( sin(Phi)*sin(Theta)/50.,   0,             -sin(Phi)*cos(Theta)/50.); break;
        case 'W': data.move( cos(Phi)*cos(Theta)/50.,   sin(Phi)/50.,   cos(Phi)*sin(Theta)/50.); break;
        case 'S': data.move(-cos(Phi)*cos(Theta)/50.,  -sin(Phi)/50.,  -cos(Phi)*sin(Theta)/50.); break;
        case 'm': r = std::max(0.01,r-0.05); break;
        case 'M': r = std::min(100.,r+0.05); break;
        default: break;
    }
}

void View::load(std::string obj_path, bool centerlize=true){    // TODO: aware of the size...
   data.set_obj(obj_path, centerlize);
}

void View::load(std::string obj_path, std::string texture_path, bool centerlize=true){    // TODO: aware of the size...
   data.set_obj(obj_path, centerlize);
   data.set_texture(texture_path);
}

void View::load(std::string obj_path, std::string vshader_path, std::string fshader_path, bool centerlize=true){
    data.set_obj(obj_path, centerlize);
    data.set_shader(vshader_path, fshader_path);
}

void View::load(std::string obj_path, std::string texture_path,  std::string vshader_path, std::string fshader_path, bool centerlize=true){
    for (int i = 0; i < obj_path.size(); i++) data_path[i] = obj_path[i];
    data.set_obj(obj_path, centerlize);
    data.set_shader(vshader_path, fshader_path);
    data.set_texture(texture_path);
}



void View::render(){    
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST); 
    glDepthFunc(GL_LESS);

    //// functions beloew not functional in OpenGL3
    // glLoadIdentity();
    // glOrtho(-dx,dx,-dy,dy,-dz,dz);

    // Give Shader the View matrix
    double Theta = theta/180.*M_PI, Phi = phi/180.*M_PI;
    double h = -r*cos(Phi), l = r*sin(Phi);
    glm::mat4x4 view = glm::lookAt(glm::vec3{l*cos(Theta),h,l*sin(Theta)}, glm::vec3{0,0,0}, glm::vec3{-h*cos(Theta),l,-h*sin(Theta)});
    data.shader.set_mat4fv("view", &view[0][0]);    

    data.render();

    if(show_cube)  drawCube(1);
    if(show_axis) drawAxis(1);
}

void error_callback(int error, const char* description){
    std::cout << "Error (" << error << ") : " << description << std::endl;
}

void View::glfw_init(){
    glfwSetErrorCallback(error_callback);
    if(!glfwInit()) { std::cout << "can not init glfw window!" << std::endl; exit(-1);} 
    window = glfwCreateWindow(windowWidth, windowHeight, "Window", NULL, NULL);
    if(!window) {std::cout << "can not creat glfw window" << std::endl; exit(-1);}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwMakeContextCurrent(window);
    // glfwSwapInterval(1);
    // glfwSetWindowCloseCallback(); // set function which act when close..
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_click_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { std::cout << "Failed to initialize GLAD!" << std::endl; exit(-1); }
}

void View::imgui_init(){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    io.Fonts->AddFontFromFileTTF("/home/qi/Desktop/Qi-GL/src/imgui/DroidSans.ttf", 24.0f);
}

void View::imgui_render(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static bool show_window_settings = false;
    static bool show_render_settings = false;

    ImGui::Begin("Settings");
    ImGui::Text("Frame Rate: %.2f ms/frame (%.1f Hz)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Checkbox("Window Settings", &show_window_settings);
    ImGui::Checkbox("Render Settings", &show_render_settings);
    
    
    ImGui::End();


    if (show_window_settings)  // general settings
    {
        ImGui::Begin("Window Settings", &show_window_settings);
        ImGui::ColorEdit3("Background Color", (float*)&clear_color);

        ImGui::End();
    }
    
    if(show_render_settings)
    {
        ImGui::Begin("Rendering Settings", &show_render_settings);
        
        if(ImGui::Button("Reload Shaders")) {data.shader.load();}
        ImGui::SameLine();
        if(ImGui::Button("Reload Texture")) {data.load_texture();}

        if(ImGui::Checkbox("Use Orth", &use_orth)) use_perp = !use_orth; 
        ImGui::SameLine();
        if(ImGui::Checkbox("Use Perp", &use_perp)) use_orth = !use_perp; 

        ImGui::Checkbox("Use Light", &use_light);

        ImGui::Checkbox("Show United Cube", &show_cube);
        ImGui::SameLine();
        ImGui::Checkbox("Show Axis", &show_axis);
        
        ImGui::SliderFloat("Radis of Camera", &r, 0.1,5);
        
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void View::imgui_end(){ // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void View::show(int argc, char **argv){
    global_view = this;
    glfw_init();
    imgui_init();
    data.load();
    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();
        this->moveMouseReact();
        this->reshapeWindowReact();
        this->render();
        glDepthFunc(GL_LESS);
        imgui_render();
        glfwSwapBuffers(window);
    }
    imgui_end();
    glfwDestroyWindow(window);
    glfwTerminate();
}
// #pragma once
#include "obj.h"

void Obj::init(){
    init_obj();
    if (use_texture) init_texture();
    if (use_shader) { 
        std::cout << "Core shading!!!" << std::endl;
        init_shader(); 
        core_bind(); 
    }
}

void Obj::init_obj(){
    std::ifstream objfile(obj_path);

    // split a string according to "t"
    std::function<std::vector<std::string>(std::string& s, char t)> split = [](std::string& s, char t){
        std::vector<std::string> ret;
        ret.push_back(std::string{});
        if (s[0] != t)   ret.back().push_back(s[0]);
        for (int i = 1; i < s.length(); i++) {
            if (s[i] != t)  ret.back().push_back(s[i]);
            else if (s[i] == t && s[i-1] != t)  ret.push_back(std::string{});
        }
        return ret;
    };

    // read the Obj file
    std::string line;
    while (std::getline(objfile, line))
    {
        std::vector<std::string> split_s = split(line, ' ');
        if      (split_s[0] == "v" )     v.push_back({atof(split_s[1].c_str()),atof(split_s[2].c_str()),atof(split_s[3].c_str())});
        else if (split_s[0] == "vt")    vt.push_back({atof(split_s[1].c_str()),atof(split_s[2].c_str())});
        else if (split_s[0] == "vn")    vn.push_back({atof(split_s[1].c_str()),atof(split_s[2].c_str()),atof(split_s[3].c_str())});
        else if (split_s[0] == "f" ) {
            if (split_s.size() != 4) std::cout << "Warning: not this(four vertex face)!!!" << std::endl;
            std::vector<std::string> f1 = split(split_s[1],'/');
            std::vector<std::string> f2 = split(split_s[2],'/');
            std::vector<std::string> f3 = split(split_s[3],'/');
            if(f1.size() >= 1)  f.push_back({atoi(f1[0].c_str()),atoi(f2[0].c_str()),atoi(f3[0].c_str())});
            if(f1.size() >= 2) ft.push_back({atoi(f1[1].c_str()),atoi(f2[1].c_str()),atoi(f3[1].c_str())});
            if(f1.size() >= 3) fn.push_back({atoi(f1[2].c_str()),atoi(f2[2].c_str()),atoi(f3[2].c_str())});
        }
        else    std::cout << "Warning: load file" << obj_path << " , meet unknow format!" << std::endl;
    }
    objfile.close();
    
    // make the index begin with zero.
    std::function<void(std::vector<glm::ivec3>&)> begin_zero = [](std::vector<glm::ivec3>& f){
        int min = 10;
        for (glm::ivec3& f_ : f) min = std::min<int>(std::min<int>(f_.x,f_.y), std::min<int>(f_.z,min));
        for (glm::ivec3& f_ : f) f_ = glm::ivec3{f_.x-min,f_.y-min,f_.z-min};
    };

    begin_zero(f);
    begin_zero(ft);
    begin_zero(fn);

    // centerlize the vertex
    glm::vec3 center{0,0,0};
    float max = 0;
    std::function<float(glm::vec3)> norm_abs = [](glm::vec3 v){
        std::function<double(double)> abs = [](double x){ if (x > 0) return x; else return -x; };
        if (abs(v.x) > abs(v.y) && abs(v.x) > abs(v.z)) return abs(v.x);
        else if(abs(v.y) > abs(v.z) )                   return abs(v.y);
        else                                            return abs(v.z);
    };
    if (centerlize)
    {
        for (glm::vec3& v_ : v) center += v_;
        center /= v.size();
        for (glm::vec3& v_ : v) { max = std::max(max,norm_abs(v_-center)); }
        scale = scale * 1./max;
    }
    
#ifdef DEV
    std::cout << "=== ===Load from " << obj_path << "=== ===" << std::endl; 
    std::cout << "v size : " << v.size() << std::endl;
    std::cout << "vt size: " << vt.size() << std::endl;
    std::cout << "vn size: " << vn.size() << std::endl;
    std::cout << "f size : " << f.size() << std::endl;
    std::cout << "ft size: " << ft.size() << std::endl;
    std::cout << "fn size: " << fn.size() << std::endl;
  if(centerlize) {        
    std::cout << "center : " << "(" << center.x << "," << center.y << "," << center.z << ")." << std::endl;   
    std::cout << "max d  : " << max << std::endl;   
    std::cout << "=== ===========(centeralized)=========== ===" << std::endl; }
  else            
    std::cout << "=== ==================================== ===" << std::endl; 
#endif
};

void Obj::init_texture(){
    int t_w, t_h, t_channel;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *pixels = stbi_load(texture_path.c_str(), &t_w, &t_h, &t_channel, 0); 
    if (pixels==NULL) { use_texture = false; std::cout << " load texture " << texture_path  <<  " failed!"; return; }
    glGenTextures(1, &texture_ID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_w, t_h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    stbi_image_free(pixels);
#ifdef DEV
    std::cout << "Load texture \"" <<  texture_path << "\" size : (h,w,c) = (" << t_h << ", " << t_w << "," << t_channel << "). to ID: " << texture_ID << "." << std::endl;
#endif
}

void Obj::init_shader(){
    // read shaders
    std::ifstream vshader_file(vshader_path);
    std::ifstream fshader_file(fshader_path);
    std::stringstream vshader_code, fshader_code;

    vshader_code << vshader_file.rdbuf();
    fshader_code << fshader_file.rdbuf();

    vshader_file.close();
    fshader_file.close();
    
    std::string vs_temp = vshader_code.str();
    std::string fs_temp = fshader_code.str();
    const char* vshader_source = vs_temp.c_str();
    const char* fshader_source = fs_temp.c_str();
    
    std::function<void(unsigned int, GLenum)> checkProgram = [](unsigned int Program_ID, GLenum flag){
        int success;
        glGetShaderiv(Program_ID, flag, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(Program_ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::ID("<< Program_ID << ")::FLAG(" << flag << ")" << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    };

    //compile them    
    vshader_ID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader_ID, 1, &vshader_source, NULL);
    glCompileShader(vshader_ID);
    checkProgram(vshader_ID, GL_COMPILE_STATUS);

    fshader_ID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader_ID, 1, &fshader_source, NULL);
    glCompileShader(fshader_ID);
    checkProgram(fshader_ID, GL_COMPILE_STATUS);

    shader_ID = glCreateProgram();
    glAttachShader(shader_ID, vshader_ID);
    glAttachShader(shader_ID, fshader_ID);
    glLinkProgram(shader_ID);
    checkProgram(shader_ID, GL_LINK_STATUS);


    std::cout << "vs ID: " << vshader_ID << std::endl;
    std::cout << "fs ID: " << fshader_ID << std::endl;
    std::cout << "s  ID: " << shader_ID  << std::endl;
    // attach them
}

void Obj::texture_settings(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 线形滤波
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 线形滤波
}

void Obj::legacy_render(){
    if (use_texture)  texture_settings();
    glColor3f(1.,1.,1.);
    glBegin(GL_TRIANGLES);
    if (use_texture)
        for (int i = 0; i < f.size(); i++){
            glm::ivec3 ti = ft[i];          glm::ivec3 fi = f[i];
            glm::vec2 ta = vt[ti.x];        glm::vec3 fa = transform(v[fi.x]); 
            glm::vec2 tb = vt[ti.y];        glm::vec3 fb = transform(v[fi.y]); 
            glm::vec2 tc = vt[ti.z];        glm::vec3 fc = transform(v[fi.z]); 
            glTexCoord2d(ta.x, ta.y);       glVertex3f(fa.x, fa.y, fa.z);
            glTexCoord2d(tb.x, tb.y);       glVertex3f(fb.x, fb.y, fb.z);
            glTexCoord2d(tc.x, tc.y);       glVertex3f(fc.x, fc.y, fc.z);
        }
    else 
        for (int i = 0; i < f.size(); i++){
            glm::ivec3 fi = f[i];
            glm::vec3 fa = transform(v[fi.x]);
            glm::vec3 fb = transform(v[fi.y]);
            glm::vec3 fc = transform(v[fi.z]);
            glVertex3f(fa.x, fa.y, fa.z);
            glVertex3f(fb.x, fb.y, fb.z);
            glVertex3f(fc.x, fc.y, fc.z);
        }
    glEnd();
    glColor3f(0,0,0);
    if(f.size() < 2000) glLineWidth(2);
    if(skeletal){
        glBegin(GL_LINES);
            for (int i = 0; i < f.size(); i++){
                glm::ivec3 fi = f[i];
                glm::vec3 fa = transform(v[fi.x]);
                glm::vec3 fb = transform(v[fi.y]);
                glm::vec3 fc = transform(v[fi.z]);
                glVertex3f(fa.x, fa.y, fa.z);
                glVertex3f(fb.x, fb.y, fb.z);
                glVertex3f(fb.x, fb.y, fb.z);
                glVertex3f(fc.x, fc.y, fc.z);
                glVertex3f(fc.x, fc.y, fc.z);
                glVertex3f(fa.x, fa.y, fa.z);
            }
        glEnd(); 
    }
}

void Obj::render(){
    if(use_shader) core_render();
    else           legacy_render();
}

void Obj::core_bind(){
    // unsigned int VBO, VAO;
    // glGenBuffers(1, &VBO);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // float vertices[] = {
    //      0.5f, -0.5f, 0.0f,  // bottom right
    //     -0.5f, -0.5f, 0.0f,  // bottom left
    //      0.0f,  0.5f, 0.0f   // top 
    // };
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glGenVertexArrays(1, &VAO);
    // glBindVertexArray(VAO);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);

    // glBindVertexArray(VAO);

    float vbo[3*(3+2)*f.size()];

    for (int i = 0; i < f.size(); i++)
    {        
        glm::vec3 va = v[f[i].x]; glm::vec2 ta = vt[ft[i].x];
        glm::vec3 vb = v[f[i].y]; glm::vec2 tb = vt[ft[i].y];
        glm::vec3 vc = v[f[i].z]; glm::vec2 tc = vt[ft[i].z]; 
        vbo[i*3*(3+2)+ 0+0]=va.x, vbo[i*3*(3+2)+ 0+1]=va.y, vbo[i*3*(3+2)+ 0+2]=va.z; vbo[i*3*(3+2)+ 0+3]=ta.x; vbo[i*3*(3+2)+ 0+4]=ta.y; 
        vbo[i*3*(3+2)+ 5+0]=vb.x, vbo[i*3*(3+2)+ 5+1]=vb.y, vbo[i*3*(3+2)+ 5+2]=vb.z; vbo[i*3*(3+2)+ 5+3]=tb.x; vbo[i*3*(3+2)+ 5+4]=tb.y; 
        vbo[i*3*(3+2)+10+0]=vc.x, vbo[i*3*(3+2)+10+1]=vc.y, vbo[i*3*(3+2)+10+2]=vc.z; vbo[i*3*(3+2)+10+3]=tc.x; vbo[i*3*(3+2)+10+4]=tc.y; 
    }

    // TODO: get data
    glGenBuffers(1, &vbo_ID);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_ID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vbo), vbo, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao_ID);
    glBindVertexArray(vao_ID);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(vao_ID);
};

void Obj::core_render(){
    glUseProgram(shader_ID);
    if( use_texture ) texture_settings();
    glBindBuffer(GL_ARRAY_BUFFER, vbo_ID);
    glBindVertexArray(vao_ID);
    int scale_ID = glGetUniformLocation(shader_ID,"scale");
    int delta_ID = glGetUniformLocation(shader_ID,"delta");
    glUniform1f(scale_ID, scale);
    glUniform3f(delta_ID, delta.x, delta.y, delta.z);
    glDrawArrays(GL_TRIANGLES, 0, 3*f.size());
};

// #pragma once
#include "obj.h"

Obj::Obj(std::string obj_path,std::string name_=" ", bool centerlize=true) : name(name_) { load_obj(obj_path,centerlize); }
 
Obj::Obj(std::string obj_path, std::string texture_path, std::string name_=" ", bool centerlize=true) : name(name_) {
    load_obj(obj_path,centerlize);
    load_texture(texture_path);
    this->texture_path = texture_path;
}

void Obj::clear_obj_data(){
    v.clear(); vt.clear(); vn.clear();
    f.clear(); ft.clear(); fn.clear();
}

void Obj::load_obj(std::string obj_path, bool centerlize){
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
    clear_obj_data();
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

void Obj::load_texture(std::string texture_path){
    if (pixels != NULL) stbi_image_free(pixels);
    this->texture_path = texture_path; 
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    pixels = stbi_load(texture_path.c_str(), &t_w, &t_h, &t_channel, 0); 
    if (pixels==NULL) { use_texture = false; std::cout << name << " load texture failed!";}
    else use_texture = true;
#ifdef DEV
    std::cout << "Image \"" <<  texture_path << "\" size : (h,w,c) = (" << t_h << ", " << t_w << "," << t_channel << ")." << std::endl;
#endif
}

void Obj::setup_texture(){
    if(texture_ID == -1) glGenTextures(1,&texture_ID);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 线形滤波
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 线形滤波
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_w, t_h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
}

void Obj::legacy_render(){
    if (use_texture)  setup_texture();
    glColor3f(1.,1.,1.);
    glBegin(GL_TRIANGLES);
    if (use_texture==true)
        for (int i = 0; i < f.size(); i++){
            glm::ivec3 ti = ft[i];          glm::ivec3 fi = f[i];
            glm::vec2 ta = vt[ti.x];        glm::vec3 fa = transform(v[fi.x]); 
            glm::vec2 tb = vt[ti.y];        glm::vec3 fb = transform(v[fi.y]); 
            glm::vec2 tc = vt[ti.z];        glm::vec3 fc = transform(v[fi.z]); 
            glTexCoord2d(ta.x, ta.y);       glVertex3f(fa.x, fa.y, fa.z);
            glTexCoord2d(tb.x, tb.y);       glVertex3f(fb.x, fb.y, fb.z);
            glTexCoord2d(tc.x, tc.y);       glVertex3f(fc.x, fc.y, fc.z);
        }
    else if(use_texture == false)
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
    // legacy_render();
}

void Obj::core_bind(){
    // glCreateBuffers();
};

void Obj::core_render(){

};
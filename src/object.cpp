// #pragma once
#include "object.h"

void Object::set_object(std::string object_path_, bool centerlize_=true) { 
    object_path=object_path_; 
    centerlize=centerlize_;
}

void Object::set_texture(std::string texture_path_) { 
    texture.set(texture_path_);
}

void Object::set_shader(std::string vshader_path_, std::string fshader_path_) {
    shader.set_path(vshader_path_, fshader_path_);
};

void Object::load(){
    load_object();
    bind(); 
    if (texture.is_setted())  texture.load();
    if (shader.is_setted())   shader.load();
}

void Object::load_object(){
    std::ifstream objectfile(object_path);

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

    // read the Object file
    std::string line;
    while (std::getline(objectfile, line))
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
        else if(split_s[0] == "#") { }
        else if(split_s[0] == "g") { }
        else if(split_s[0] == "usemtl") {}
        else   {std::cout << "Warning: load file" << object_path << " , meet unknow format! Please Update this...(" << line  << ")."<< std::endl;}
    }
    objectfile.close();
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
        for (glm::vec3& v_ : v) { max = std::max(max , norm_abs(v_-center) ); }
        scale = scale * 1./max;
    }
};

void Object::bind(){   // need refine into elementary

    // std::vector<float> vbo(3*(3+2)*f.size(),0);
    float vbo[3*(3+2+3)*f.size()];


    for (int i = 0; i < f.size(); i++)
    {
        glm::vec3 va = v[f[i].x]; 
        glm::vec3 vb = v[f[i].y]; 
        glm::vec3 vc = v[f[i].z]; 
        vbo[i*3*(3+2+3)+ 0+0]=va.x, vbo[i*3*(3+2+3)+ 0+1]=va.y, vbo[i*3*(3+2+3)+ 0+2]=va.z; 
        vbo[i*3*(3+2+3)+ 8+0]=vb.x, vbo[i*3*(3+2+3)+ 8+1]=vb.y, vbo[i*3*(3+2+3)+ 8+2]=vb.z; 
        vbo[i*3*(3+2+3)+16+0]=vc.x, vbo[i*3*(3+2+3)+16+1]=vc.y, vbo[i*3*(3+2+3)+16+2]=vc.z; 
    }
    
    if( texture.is_setted() && vt.size() != 0 ) {
        for (int i = 0; i < f.size(); i++)
        {        
            glm::vec2 ta = vt[ft[i].x];
            glm::vec2 tb = vt[ft[i].y];
            glm::vec2 tc = vt[ft[i].z]; 
            vbo[i*3*(3+2+3)+ 0+3]=ta.x; vbo[i*3*(3+2+3)+ 0+4]=ta.y; 
            vbo[i*3*(3+2+3)+ 8+3]=tb.x; vbo[i*3*(3+2+3)+ 8+4]=tb.y; 
            vbo[i*3*(3+2+3)+16+3]=tc.x; vbo[i*3*(3+2+3)+16+4]=tc.y; 
        }
    }
    else{
        for (int i = 0; i < f.size(); i++)
        {        
            vbo[i*3*(3+2+3)+ 0+3]=0; vbo[i*3*(3+2+3)+ 0+4]=0; 
            vbo[i*3*(3+2+3)+ 8+3]=0; vbo[i*3*(3+2+3)+ 8+4]=0; 
            vbo[i*3*(3+2+3)+16+3]=0; vbo[i*3*(3+2+3)+16+4]=0; 
        }
    }

    if( texture.is_setted() && vn.size() != 0 )
    for (int i = 0; i < f.size(); i++)
    {        
        glm::vec2 ta = vt[ft[i].x];
        glm::vec2 tb = vt[ft[i].y];
        glm::vec2 tc = vt[ft[i].z]; 
        vbo[i*3*(3+2+3)+ 0+3]=ta.x; vbo[i*3*(3+2+3)+ 0+4]=ta.y; 
        vbo[i*3*(3+2+3)+ 8+3]=tb.x; vbo[i*3*(3+2+3)+ 8+4]=tb.y; 
        vbo[i*3*(3+2+3)+16+3]=tc.x; vbo[i*3*(3+2+3)+16+4]=tc.y; 
    }
    // std::cout << "Nothing ghere" << std::endl;
    glGenBuffers(1, &vbo_ID);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_ID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vbo), vbo, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao_ID);
    glBindVertexArray(vao_ID);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
};

void Object::render(){

    shader.use();
    shader.set_1f("scale", scale);
    shader.set_3f("delta", delta.x, delta.y, delta.z);

    if( texture.is_setted() ) texture.use();
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo_ID);
    glBindVertexArray(vao_ID);
    if(skeletal)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, 3*f.size());
};


std::string Object::info(){
    std::string ret;
    ret += "Object info: Name: " + name + "\t, From: " + object_path + "\n"; 
    ret += "\t # Vertices("+std::to_string(v.size())   + "),\t # Faces(" + std::to_string(f.size())  + ").\t\n";
    ret += "\t #tVertices("+std::to_string(vt.size())  + "),\t #tFaces(" + std::to_string(ft.size()) + ").\t\n";
    ret += "\t #nVertices("+std::to_string(vn.size())  + "),\t #nFaces(" + std::to_string(fn.size()) + ").\t\n";
    ret += "\t Center: (" + std::to_string(center.x) + "," + std::to_string(center.y) + "," + std::to_string(center.z) + ").\n";   
    ret += "\tR: " + std::to_string(max) + "\n";   
    // ret += shader.info();
    // ret += texture.info(); 
  if(centerlize) { ret += "(centeralized)\n"; }
    return ret;
}

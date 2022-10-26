#pragma once

#include <iostream>
#include <fstream>

struct vec2d
{
public:
    double u;
    double v;
public:
    vec2d(){};
    vec2d(double x_, double y_){
        u = x_;
        v = y_;
    };
    ~vec2d(){};
    void print(){ std::cout << "(" << u << "," << v << ")";  }
    vec2d operator+(vec2d a){
        return vec2d{a.u+u, a.v+v};
    }
    vec2d operator-(vec2d a){
        return vec2d{u-a.u, v-a.v};
    }
    vec2d operator*(double a){
        return vec2d{a*u, a*v};
    }
    vec2d operator/(double a){
        return vec2d{u/a, v/a};
    }
    double max(){
        if (abs(u) > abs(v)) return abs(u);
        else                 return abs(v);
    }
};

struct vec2i
{
public:
    int x;
    int y;
public:
    vec2i(){};
    vec2i(int x_, int y_) : x(x_), y(y_){};
    ~vec2i(){};
    void print(){ std::cout << "(" << x << "," << y << ")";  }
};

struct vec3d
{
public:
    double x;
    double y;
    double z;
public:
    vec3d(){};
    vec3d(double x_, double y_, double z_){
        x = x_;
        y = y_;
        z = z_;
    };
    ~vec3d(){};
    void print(){ std::cout << "(" << x << "," << y << "," << z << ")";  }
    vec3d operator+(vec3d a){
        return vec3d{a.x+x, a.y+y, a.z+z};
    }
    vec3d operator-(vec3d a){
        return vec3d{x-a.x, y-a.y, z-a.z};
    }
    vec3d operator*(double a){
        return vec3d{a*x, a*y, a*z};
    }
    vec3d operator/(double a){
        return vec3d{x/a, y/a, z/a};
    }
    double max(){
        if (abs(x) > abs(y) && abs(x) > abs(z)) return abs(x);
        else if(abs(y) > abs(z) )               return abs(y);
        else                                    return abs(z);
    }
};

struct vec3i
{
public:
    unsigned int a;
    unsigned int b;
    unsigned int c;
public:
    vec3i(){};
    vec3i(double a_, double b_, double c_){
        a = a_;
        b = b_;
        c = c_;
    };
    vec3i(unsigned int a_, unsigned int b_, unsigned int c_):a(a_), b(b_),c(c_){};
    vec3i(int a_, int b_, int c_) : a(a_), b(b_),c(c_){};
    ~vec3i(){};
    void print(){ std::cout << "(" << a << "," << b << "," << c << ")";  }

};


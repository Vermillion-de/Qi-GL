#pragma once
#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cmath>
#include <map>


#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/vec3.hpp>

#include "common.cpp"

glm::vec3 cross(glm::vec3 a, glm::vec3 b)
{
    return {a.y*b.z - a.z*b.y, b.x*a.z - a.x*b.z, a.x*b.y - a.y*b.x};
}

glm::vec3 normalize(glm::vec3 a){
    double length = sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
    return {a.x/length, a.y/length, a.z/length};
}

double norm(glm::vec3 a){
    return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

std::vector<glm::vec3> normal_gen(std::vector<glm::vec3>& v, std::vector<glm::ivec3>& f){
    std::vector<glm::vec3> vn(v.size(), {0,0,0});
    for (int i = 0; i < f.size(); i++)
    {
        glm::vec3 a = cross(v[f[i].x] - v[f[i].y], v[f[i].x] - v[f[i].z]);
        vn[f[i].x] += a;
        vn[f[i].y] += a;
        vn[f[i].z] += a;
    }
    for (int i = 0; i < vn.size(); i++)
    {
        vn[i] = normalize(vn[i]);
    }
    return vn;
}
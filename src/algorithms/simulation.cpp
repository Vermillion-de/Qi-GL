#include <cmath>
#include "halfedge.cpp"

glm::vec3 prod(glm::vec3 a, double r){
    return {a.x*r, a.y*r, a.z*r};
}

double abs(glm::vec3 a){
    return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

glm::vec3 dx(glm::vec3 a, double l0){
    double a_norm = sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
    glm::vec3 a_normalized = prod(a, 1./a_norm);
    return prod(a_normalized, a_norm-l0);
}

void simulate(std::vector<glm::vec3>& vert, std::vector<glm::ivec3> face, double l0 = 1, double k=100000, double step=0.0006){
    static std::vector<glm::vec3> v(vert.size(), {0,0,0});
    static Halfedge h(vert, face);

    std::vector<glm::vec3> f(vert.size(), {0,0,0});

    for (int i = 0; i < face.size(); i++)
    {
        int a = face[i].x , b = face[i].y, c = face[i].z;
        f[a] += prod(dx( vert[a]-vert[b], l0),-k) + prod(dx( vert[a]-vert[c],l0),-k) + prod(v[a], -0.5*abs(v[a])); 
        f[b] += prod(dx( vert[b]-vert[a], l0),-k) + prod(dx( vert[b]-vert[c],l0),-k) + prod(v[b], -0.5*abs(v[b])); 
        f[c] += prod(dx( vert[c]-vert[b], l0),-k) + prod(dx( vert[c]-vert[a],l0),-k) + prod(v[c], -0.5*abs(v[c])); 
    }

    std::vector<int> boundary = h.getBoundary();
    for (int i = 0; i < boundary.size(); i++)
        f[boundary[i]] = {0,0,0};
    
    
    for (int i = 0; i < v.size(); i++)
    {
        v[i] += prod(f[i], step);
    }
    
    for (int i = 0; i < v.size(); i++)
    {
        vert[i] += prod(v[i], step);
    }
}
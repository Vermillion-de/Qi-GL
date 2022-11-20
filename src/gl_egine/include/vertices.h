#include <vector>


#include <GL/freeglut.h>
#include <glm/glm.hpp>

class Vertices
{
private:
    int vao_ID;
    int vbo_ID;
    int veo_ID;
    int n_faces;
    float *vbo;
private:
    void assign(float* vbo_, unsigned int element, unsigned delta, std::vector<glm::vec3>& vx, std::vector<glm::ivec3> fx){
        for (int i = 0; i < fx.size(); i++)
        {
            glm::vec3 va = v[f[i].x]; 
            glm::vec3 vb = v[f[i].y]; 
            glm::vec3 vc = v[f[i].z]; 
            vbo[i*3*element + element*0 + delta + 0]=va.x, vbo[i*3*element + element*0 + delta + 1]=va.y, vbo[i*3*element + element*0 + delta + 2]=va.z; 
            vbo[i*3*element + element*1 + delta + 0]=va.x, vbo[i*3*element + element*1 + delta + 1]=va.y, vbo[i*3*element + element*1 + delta + 2]=va.z; 
            vbo[i*3*element + element*2 + delta + 0]=va.x, vbo[i*3*element + element*2 + delta + 1]=va.y, vbo[i*3*element + element*2 + delta + 2]=va.z; 
        }        
    }

    void assign(float* vbo_, unsigned int element, unsigned delta, std::vector<glm::vec2>& vx, std::vector<glm::ivec3> fx){
        for (int i = 0; i < fx.size(); i++)
        {
            glm::vec3 va = v[f[i].x]; 
            glm::vec3 vb = v[f[i].y]; 
            glm::vec3 vc = v[f[i].z]; 
            vbo[i*3*element + element*0 + delta + 0]=va.x, vbo[i*3*element + element*0 + delta + 1]=va.y; 
            vbo[i*3*element + element*1 + delta + 0]=va.x, vbo[i*3*element + element*1 + delta + 1]=va.y; 
            vbo[i*3*element + element*2 + delta + 0]=va.x, vbo[i*3*element + element*2 + delta + 1]=va.y; 
        }        
    }

public:
    Vertices(std::vector<glm::vec3>& v, std::vector<glm::vec3>& f){
        vbo = new float[3*(3+2+3)*f.size()];
        assign(vbo, 8, 0, v, f);
        n_faces = f.size();
    };
    Vertices(std::vector<glm::vec3>& v, std::vector<glm::vec3>& f, std::vector<glm::vec3>& vt, std::vector<glm::vec3>& ft){
        vbo = new float[3*(3+2+3)*f.size()];
        assign(vbo, 8, 0, v, f);
        n_faces = f.size();
        assign(vbo, 8, 3, vt, ft);

    }
    Vertices(std::vector<glm::vec3>& v, std::vector<glm::vec3>& f, std::vector<glm::vec3>& vt, std::vector<glm::vec3>& ft, std::vector<glm::vec3>& vn, std::vector<glm::vec3>& fn){
        vbo = new float[3*(3+2+3)*f.size()];
        assign(vbo, 8, 0, v, f);
        n_faces = f.size();
        assign(vbo, 8, 3, vt, ft);
        assign(vbo, 8, 5, vn, fn);
    }
    ~Vertices(){};
public:
    void load(){
        glGenBuffers(1, &vbo_ID);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vbo), vbo, GL_STATIC_DRAW);
        delete vbo;

        glGenVertexArrays(1, &vao_ID);
        glBindVertexArray(vao_ID);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
    };
    void bind(){
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ID);
        glBindVertexArray(vao_ID);
    }
    void render(){
        if(false)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, 3*f.size());
    }
};

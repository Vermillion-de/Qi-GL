#include <vector>


#include <GL/freeglut.h>
#include <glm/glm.hpp>

class Vertices
{
private:
    int vao_ID;
    int vbo_ID;
    int veo_ID;
public:
    Vertices(std::vector<glm::vec3>& v, std::vector<glm::vec3>& f){
        float a;
    };
    Vertices(std::vector<glm::vec3>& v, std::vector<glm::vec3>& f, std::vector<glm::vec3>& vt, std::vector<glm::vec3>& ft){

    }
    
    Vertices(std::vector<glm::vec3>& v, std::vector<glm::vec3>& f, std::vector<glm::vec3>& vt, std::vector<glm::vec3>& ft, std::vector<glm::vec3>& vn, std::vector<glm::vec3>& fn){
        
    }
    ~Vertices(){};
public:
    void load(){

    };
    void bind(){

    }
};

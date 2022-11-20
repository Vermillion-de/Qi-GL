#include "halfedge.cpp"
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <glm/vec2.hpp>

using Triple = Eigen::Triplet<double>;
using SpMat = Eigen::SparseMatrix<double>;

double norm_(glm::vec3 a){
    return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

double dot_(glm::vec3 a, glm::vec3 b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

double angle(glm::vec3 a, glm::vec3 b){
    return acos(dot_(a,b)/(norm_(a)*norm_(b)));
}

struct Circle
{
    int N;
    Circle(int N_){
        N = N_;
    }
    glm::vec3 gen(int n){
        return {cos(n*2*M_PI/ N)/2+0.5,sin(n*2*M_PI/N)/2+0.5,0};
    }
    ~Circle(){}
};

void compute_J(std::vector<glm::vec3>& v, std::vector<glm::vec2>& u, std::vector<glm::ivec3> f, std::vector<Eigen::Matrix2d>& J, std::vector<Eigen::Vector2d>& B){
    // old verteces: x ==> new vertices u, estimate Jx+B=u;
    J.resize(f.size()), B.resize(f.size());

    Eigen::MatrixXd U(2,3), X(3,3);
    for (int i = 0; i < f.size(); i++)
    {
        glm::vec3 x1_ = v[f[i].x], x2_ = v[f[i].y], x3_ = v[f[i].z];
        double l2 = norm_(x1_ - x2_), l3 = norm_(x1_ - x3_);
        double theta = angle(x1_ - x2_, x1_ - x3_);
        glm::vec2 x1 = {0,0}, x2 = {l2, 0}, x3 = {l3*cos(theta), l3*sin(theta)};

        glm::vec2 u1 = u[f[i].x], u2 = u[f[i].y], u3 = u[f[i].z];
        X(0,0) = x1.x, X(0,1) = x2.x, X(0,2) = x3.x;
        X(1,0) = x1.y, X(1,1) = x2.y, X(1,2) = x3.y;
        X(2,0) = 1   , X(2,1) = 1   , X(2,2) = 1   ;

        U(0,0) = u1.x, U(0,1) = u2.x, U(0,2) = u3.x; 
        U(1,0) = u1.y, U(1,1) = u2.y, U(1,2) = u3.y; 

        Eigen::MatrixXd JB = U*X.inverse();
        J[i](0,0) = JB(0,0), J[i](0,1) = JB(0,1);
        J[i](1,0) = JB(1,0), J[i](1,1) = JB(1,1);

        B[i](0) = JB(0,2);
        B[i](1) = JB(1,2);

        std::cout << J[i] << std::endl;
        std::cout << B[i] << std::endl;
    }
}

void asap(std::vector<glm::vec3>& v, std::vector<glm::ivec3>& f){
 Halfedge h(v,f);
    std::vector<int> boundary = h.getBoundary();
    std::vector<bool> is_boundary(v.size(), false);
    for (int i = 0; i < boundary.size(); i++)
        is_boundary[boundary[i]] = true;

    SpMat A(v.size(), v.size());
    Eigen::MatrixXd b(v.size(),2);
    std::vector<Triple> coef; 

    // generate Laplacian term
    for (int i = 0; i < is_boundary.size(); i++)
    {
        if (! is_boundary[i]) {
            std::vector<int> circle = h.getCircle(i);
            coef.push_back({i,i,double(circle.size())});
            for (int j = 0; j < circle.size(); j++)
                coef.push_back({i,circle[j],-1.});
        }
    }

    // generate boundary tern
    Circle texture_edge(boundary.size());
    for (int i = 0; i < boundary.size(); i++)
    {
        coef.push_back({boundary[i],boundary[i],1});
        glm::vec2 t = texture_edge.gen(i);
        b(boundary[i],0) = t[0];         
        b(boundary[i],1) = t[1];         
    }
    
    A.setFromTriplets(coef.begin(), coef.end());
    
    Eigen::LeastSquaresConjugateGradient<SpMat> solver(A); 
    Eigen::MatrixXd x = solver.solve(b);

    for (int i = 0; i < v.size(); i++)
    {
        // vt.push_back({x(i,0),x(i,1)});
        v[i] = {x(i, 0),x(i,1),0};
    }
}
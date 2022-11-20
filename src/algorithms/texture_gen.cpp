#pragma once
#include <Eigen/Sparse>
#include <Eigen/SparseQR>
#include <Eigen/Dense>
#include <glm/vec2.hpp>
#include "halfedge.cpp"

using Triple = Eigen::Triplet<double>;
using SpMat = Eigen::SparseMatrix<double>;

struct Circler
{
    int N;
    int quater;
    double d_last;
    Circler(int N_){
        N = N_;
        quater = N/4;
        d_last = 1./(N - 3*quater);
    }
    glm::vec3 gen(int n){
        assert(n >=0 && n < N);
        if (n < quater)
        {
            return {n * 1./quater, 0, 0};
        }
        else if (n < 2 * quater)
        {
            return {1, (n%quater) * 1./quater, 0};
        }
        else if (n < 3 * quater)
        {
            return {1 - (n%quater) * 1./quater, 1, 0};
        }
        else {
            return {0, 1 - d_last * (n%quater), 0};
        }
        // return {cos(n*2*M_PI/ N)/2+0.5,sin(n*2*M_PI/N)/2+0.5,0};
    }
    ~Circler(){}
};


std::vector<glm::vec2> texture_gen(std::vector<glm::vec3>& v, std::vector<glm::ivec3>& f){
    Halfedge h(v,f);
    std::vector<int> boundary = h.getBoundary();
    std::vector<bool> is_boundary(v.size(), false);
    for (int i = 0; i < boundary.size(); i++)
        is_boundary[boundary[i]] = true;

    SpMat A(v.size(), v.size());
    Eigen::MatrixXd b(v.size(), 2);
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
    Circler texture_edge(boundary.size());
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

    std::vector<glm::vec2> vt;

    for (int i = 0; i < v.size(); i++)
    {
        vt.push_back({x(i,0),x(i,1)});
            // v[i] = {x(i,0),x(i,1),0};
    }
    return vt;
}


std::vector<glm::vec2> texture_gen_cot(std::vector<glm::vec3>& v, std::vector<glm::ivec3>& f){
    Halfedge h(v,f);
    std::vector<int> boundary = h.getBoundary();
    std::vector<bool> is_boundary(v.size(), false);
    for (int i = 0; i < boundary.size(); i++)
        is_boundary[boundary[i]] = true;

    SpMat A(v.size(), v.size());
    Eigen::MatrixXd b(v.size(), 2);
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
    Circler texture_edge(boundary.size());
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

    std::vector<glm::vec2> vt;

    for (int i = 0; i < v.size(); i++)
    {
        vt.push_back({x(i,0),x(i,1)});
            // v[i] = {x(i,0),x(i,1),0};
    }
    return vt;
}
#pragma once
#include <Eigen/Sparse>
#include <Eigen/SparseQR>
#include <Eigen/Dense>
#include "halfedge.cpp"

using Triple = Eigen::Triplet<double>;
using SpMat = Eigen::SparseMatrix<double>;

void minisurface(std::vector<glm::vec3>& v, std::vector<glm::ivec3>& f){
    Halfedge h(v,f);
    std::vector<int> bound = h.getBoundary();
    std::vector<bool> is_bound(v.size(), false);
    for (int i = 0; i < bound.size(); i++)
        is_bound[bound[i]] = true;

    SpMat A(v.size(), v.size());
    Eigen::MatrixXd b(v.size(), 3);
    std::vector<Triple> coef; 
    for (int i = 0; i < is_bound.size(); i++)
    {
        if (is_bound[i])
        {
            coef.push_back({i,i,1});
            b(i,0) = v[i][0];
            b(i,1) = v[i][1];
            b(i,2) = v[i][2];
        }
        else {
            std::vector<int> circle = h.getCircle(i);
            coef.push_back({i,i,double(circle.size())});
            for (int j = 0; j < circle.size(); j++)
                coef.push_back({i,circle[j],-1.});
        }
    }
    A.setFromTriplets(coef.begin(), coef.end());
    
    Eigen::LeastSquaresConjugateGradient<SpMat> solver(A); 
    Eigen::MatrixXd x = solver.solve(b);
    
    for (int i = 0; i < v.size(); i++)
    {
        // if(!is_bound[i])
            v[i] = {x(i,0),x(i,1),x(i,2)};
    }
    
}


void minisurface_cot(std::vector<glm::vec3>& v, std::vector<glm::ivec3>& f){
    Halfedge h(v,f);
    std::vector<int> bound = h.getBoundary();
    std::vector<bool> is_bound(v.size(), false);
    for (int i = 0; i < bound.size(); i++)
        is_bound[bound[i]] = true;

    SpMat A(v.size(), v.size());
    Eigen::MatrixXd b(v.size(), 3);
    std::vector<Triple> coef; 
    for (int i = 0; i < is_bound.size(); i++)
    {
        if (is_bound[i])
        {
            coef.push_back({i,i,1});
            b(i,0) = v[i][0];
            b(i,1) = v[i][1];
            b(i,2) = v[i][2];
        }
        else {
            std::vector<int> circle = h.getCircle(i);
            coef.push_back({i,i,double(circle.size())});
            for (int j = 0; j < circle.size(); j++)
                coef.push_back({i,circle[j],-1.});
        }
    }
    A.setFromTriplets(coef.begin(), coef.end());
    
    Eigen::LeastSquaresConjugateGradient<SpMat> solver(A); 
    Eigen::MatrixXd x = solver.solve(b);
    
    for (int i = 0; i < v.size(); i++)
    {
        // if(!is_bound[i])
            v[i] = {x(i,0),x(i,1),x(i,2)};
    }
    
}
#pragma once
#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdio>
#include <map>

#include <glm/vec3.hpp>

class Halfedge
{
private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::ivec3> faces;
    std::vector<std::vector<std::pair<int,int>>> ngh_tri;
    std::vector<std::vector<int>> nghs;

    std::vector<std::vector<int>> idx2faces;
    
    std::vector<int> boundary;

public:
    Halfedge(){};
    Halfedge(std::vector<glm::vec3> & vert, std::vector<glm::ivec3>& fs)
        : vertices(vert), faces(fs)
    {
        // init nghs
        ngh_tri.resize(vert.size()), idx2faces.resize(vert.size());
        for (int i = 0; i < fs.size(); i++)
        {
            ngh_tri[fs[i][0]].push_back({fs[i][1],fs[i][2]}), idx2faces[fs[i][0]].push_back(i);
            ngh_tri[fs[i][1]].push_back({fs[i][2],fs[i][0]}), idx2faces[fs[i][1]].push_back(i);
            ngh_tri[fs[i][2]].push_back({fs[i][0],fs[i][1]}), idx2faces[fs[i][2]].push_back(i);
        }
        mk_nghs();

        // init boundarys
        std::map<std::pair<int,int>, int> boundary_lines;
        for (int i = 0; i < fs.size(); i++)
        {
            boundary_lines[{fs[i][0], fs[i][1]}] += 1; boundary_lines[{fs[i][1],fs[i][0]}] += 1;
            boundary_lines[{fs[i][1], fs[i][2]}] += 1; boundary_lines[{fs[i][2],fs[i][1]}] += 1;
            boundary_lines[{fs[i][2], fs[i][0]}] += 1; boundary_lines[{fs[i][0],fs[i][2]}] += 1;
        }
        std::vector<std::pair<int,int>> boundary_circle;
        for (auto boundary_ : boundary_lines)
        {
            if( boundary_.second == 1 && boundary_.first.first < boundary_.first.second){
                boundary_circle.push_back(boundary_.first);
            }
        }
        // std::cout << "BoundaryCircle size: " << boundary_circle.size() << std::endl;
        // for (int i = 0; i < boundary_circle.size(); i++)
        // {
        //     std::cout << "(" << boundary_circle[i].first << "," << boundary_circle[i].second << ") => ";
        // }
        // std::cout << std::endl;
        boundary = mkCircle(boundary_circle);
        // print_nghTris();
    }
    ~Halfedge(){};
public:
    std::vector<int> getCircle(int i){
        if (i < 0 || i > nghs.size()) return {};
        return nghs[i];
    }
    std::vector<int> getFaces(int i){
        if (i < 0 || i > nghs.size()) return {};
        return idx2faces[i];
    }
    std::vector<int> getBoundary(){
        return boundary;
    } 

public:
    std::vector<int> mkCircle(std::vector<std::pair<int,int>> & tris){
        std::vector<int> ret{tris[0].first, tris[0].second};
        int temp = ret.back(), p = 0;
        for (int i = 1; i < tris.size() - 1; i++){
            for (int j = 0; j < tris.size() ; j++)
            {
                if (tris[j].first == temp && j != p){
                    p = j;
                    ret.push_back(tris[j].second);
                    break;
                }
                if (tris[j].second == temp && j != p)
                {
                    p = j;
                    ret.push_back(tris[j].first);
                    break;
                }
            }
            temp = ret.back();
        }
        return ret;
    }
    
    std::vector<int> mkCircle(std::vector<std::pair<int,int>>& tris, std::vector<int>& faces){
        std::vector<int> ret{tris[0].first, tris[0].second};
        
        std::vector<int> faces_;

        int temp = ret.back(), p = 0;
        faces_.push_back(faces[p]);
        for (int i = 1; i < tris.size() - 1; i++){
            for (int j = 0; j < tris.size() ; j++)
            {
                if (tris[j].first == temp && j != p){
                    p = j;
                    ret.push_back(tris[j].second);
                    break;
                }
                if (tris[j].second == temp && j != p)
                {
                    p = j;
                    ret.push_back(tris[j].first);
                    break;
                }
            }
            temp = ret.back();
            faces_.push_back(faces[p]);
        }
        faces = faces_;
        return ret;
    }

    void mk_nghs(){
        for (int i = 0; i < ngh_tri.size(); i++) 
            nghs.push_back( mkCircle(ngh_tri[i],idx2faces[i]));
    }
    void print_nghTris(){
        for (int i = 0; i < ngh_tri.size(); i++)
        {
            std::cout << "the " << i << " to ";
            for (int j = 0; j < ngh_tri[i].size(); j++)
            {
                std::cout << " => (" << ngh_tri[i][j].first << "," << ngh_tri[i][j].second << ") " << "[" << faces[idx2faces[i][j]][0] << "," << faces[idx2faces[i][j]][1] << "," << faces[idx2faces[i][j]][2] << "]" ;  ;  
            }
            std::cout << std::endl;

            std::cout << "the " << i << " th circle ";
            for (int j = 0; j < nghs[i].size(); j++)
            {
                std::cout << "=> " << nghs[i][j] << " ";
            }
            std::cout << std::endl;            
        }   
    }
};
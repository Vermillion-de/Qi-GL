#include <stdlib.h>
#include <string.h>

#define USE_QIView
#include "view.h"
#undef USE_QIView

int main(int argc, char **argv){

    std::map<std::string, std::string> config_cow{
        {"object", "../data/spot.obj"}, 
        {"centerlize", "true"},
        {"texture" , "../data/cow.png"},
        {"vshader", "/home/qi/Desktop/Qi-GL/src/shaders/vshader.vs"},
        {"fshader", "/home/qi/Desktop/Qi-GL/src/shaders/fshader.fs"}
    };

    std::map<std::string, std::string> config_orge{
        {"object", "../data/orge/bs_angry.obj"}, 
        {"centerlize", "true"},
        {"texture" , "../data/orge/diffuse.png"},
        {"vshader", "/home/qi/Desktop/Qi-GL/src/shaders/vshader.vs"},
        {"fshader", "/home/qi/Desktop/Qi-GL/src/shaders/fshader.fs"}
    };

    View a;
    a.load(config_orge);
    // a.load("../data/lucy.obj", true);
    // a.load("../data/orge/bs_angry.obj",true);
    // a.load("../data/orge/bs_angry.obj","../data/orge/diffuse.png",true);
    // a.load("../data/orge/bs_angry.obj","../data/orge/diffuse.png","/home/qi/Desktop/Qi-GL/src/shaders/vshader.vs","/home/qi/Desktop/Qi-GL/src/shaders/fshader.fs", true);
    // a.load("../data/spot.obj","../data/cow.png","/home/qi/Desktop/Qi-GL/src/shaders/vshader.vs","/home/qi/Desktop/Qi-GL/src/shaders/fshader.fs", true);
    a.show(argc, argv);
    return 0;
}
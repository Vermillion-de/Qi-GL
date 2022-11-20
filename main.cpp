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
        {"vshader", "/home/qi/Desktop/Qi-GL/src/resources/shaders/vshader.vs"},
        {"fshader", "/home/qi/Desktop/Qi-GL/src/resources/shaders/fshader.fs"}
    };

    std::map<std::string, std::string> config_orge{
        {"object", "../data/orge/bs_angry.obj"}, 
        {"centerlize", "true"},
        {"texture" , "../data/orge/diffuse.png"},
        {"vshader", "/home/qi/Desktop/Qi-GL/src/resources/shaders/vshader.vs"},
        {"fshader", "/home/qi/Desktop/Qi-GL/src/resources/shaders/fshader.fs"}
    };

    std::map<std::string, std::string> config_minisurface{
        {"object", "../data/minisurface/Balls.obj"}, 
        {"centerlize", "true"},
        {"texture" , "../data/minisurface/test.png"},
        {"vshader", "/home/qi/Desktop/Qi-GL/src/resources/shaders/vshader.vs"},
        {"fshader", "/home/qi/Desktop/Qi-GL/src/resources/shaders/fshader.fs"}
    };

    std::map<std::string, std::string> config_bunny{
        {"object", "../data/minisurface/Bunny_head.obj"}, 
        {"centerlize", "true"},
        {"texture" , "../data/minisurface/test.png"},
        {"vshader", "/home/qi/Desktop/Qi-GL/src/resources/shaders/vshader.vs"},
        {"fshader", "/home/qi/Desktop/Qi-GL/src/resources/shaders/fshader.fs"}
    };

    std::map<std::string, std::string> config_simu{
        {"object", "../data/grid.obj"}, 
        {"centerlize", "true"},
        {"texture" , "../data/minisurface/test.png"},
        {"vshader", "/home/qi/Desktop/Qi-GL/src/resources/shaders/vshader.vs"},
        {"fshader", "/home/qi/Desktop/Qi-GL/src/resources/shaders/fshader.fs"}
    };

    std::map<std::string, std::string> config_wuson{
        {"object", "../data/Wuson.obj"}, 
        {"centerlize", "true"},
        {"texture" , "../data/minisurface/test.png"},
        {"vshader", "/home/qi/Desktop/Qi-GL/src/resources/shaders/vshader.vs"},
        {"fshader", "/home/qi/Desktop/Qi-GL/src/resources/shaders/fshader.fs"}
    };


    std::map<std::string, std::string> config_teapot{
        {"object", "../data/teapot.obj"}, 
        {"centerlize", "true"},
        {"texture" , "../data/minisurface/test.png"},
        {"vshader", "/home/qi/Desktop/Qi-GL/src/resources/shaders/vshader.vs"},
        {"fshader", "/home/qi/Desktop/Qi-GL/src/resources/shaders/fshader.fs"}
    };

    View a;
    a.load(config_orge);
    a.show(argc, argv);
    return 0;
}
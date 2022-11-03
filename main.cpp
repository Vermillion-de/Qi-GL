#include <stdlib.h>
#include <string.h>

#define USE_QIView
#include "view.h"
#undef USE_QIView

int main(int argc, char **argv){
    View a;
    // a.load("../data/orge/bs_angry.obj",true);
    // a.load("../data/orge/bs_angry.obj","../data/orge/diffuse.png",true);
    // a.load("../data/orge/bs_angry.obj","../data/orge/diffuse.png","/home/qi/Desktop/Qi-GL/src/shaders/vshader.vs","/home/qi/Desktop/Qi-GL/src/shaders/fshader.fs", true);
    a.load("../data/spot.obj","../data/cow.png","/home/qi/Desktop/Qi-GL/src/shaders/vshader.vs","/home/qi/Desktop/Qi-GL/src/shaders/fshader.fs", true);
    a.show(argc, argv);
    return 0;
}
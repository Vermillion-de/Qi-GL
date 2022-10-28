#include <stdlib.h>
#include <string.h>
#include "./src/view.cpp"

int main(int argc, char **argv){
    View a;
    a.load("./data/orge/bs_angry.obj","./data/orge/diffuse.png",true);
    a.show(argc, argv);
    return 0;
}
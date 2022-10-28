#include <stdlib.h>

#include "view.cpp"

int main(int argc, char **argv){
    View a;
    a.load("./data/orge/bs_angry.obj");
    a.show(argc, argv);
    return 0;
}
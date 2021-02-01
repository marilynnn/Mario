#include "floor.h"
#include <iostream>

Floor::Floor(int x, int y ) : Block(x,y, ":images/floor.png")
{}

Floor::~Floor()
{
    std::cout << ("Floor deleted\n");
}

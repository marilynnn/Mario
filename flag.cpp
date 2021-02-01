#include "flag.h"
#include <iostream>

Flag::Flag(int x, int y ) : Block(x,y, ":images/flag.png")
{}

Flag::~Flag()
{
    std::cout << ("Flag deleted\n");
}

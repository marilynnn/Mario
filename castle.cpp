#include "castle.h"

#include <iostream>

Castle::Castle(int x, int y) : Block(x, y, ":images/castle.png")
{}

Castle::~Castle()
{
    std::cout << ("Castle deleted\n");
}

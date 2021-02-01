#include "spike.h"
#include <iostream>

Spike::Spike(int x, int y ) : Block(x,y, ":images/spike.png")
{}

Spike::~Spike()
{
    std::cout << ("Spike deleted\n");
}

#include "won.h"
#include <iostream>

Won::Won(int x, int y ) : Block(x,y, ":images/won.png")
{
    std::cout << ("Woncreated\n");
}

Won::~Won()
{
    std::cout << ("Won deleted\n");
}

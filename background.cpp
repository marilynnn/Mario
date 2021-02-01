#include "background.h"
#include <iostream>

Background::Background(int x, int y ) : Block(x,y, ":images/background.png")
{}

Background::~Background()
{
    std::cout << ("Background deleted\n");
}

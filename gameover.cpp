#include "gameover.h"
#include <iostream>

GameOver::GameOver(int x, int y ) : Block(x,y, ":images/gameover.png")
{
    std::cout << ("GameOver created\n");
}

GameOver::~GameOver()
{
    std::cout << ("GameOver deleted\n");
}

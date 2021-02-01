#include "wingedgoomba.h"
#include <iostream>

WingedGoomba::WingedGoomba(int x, int y) : Monster(x, y)
{
    this->setLife(1);
    this->moveLSprite = QPixmap(":images/wingedgoomba.png");
    this->rect = QRect(x, y, 60, moveLSprite.size().height());
}

WingedGoomba::~WingedGoomba()
{
    std::cout << ("WingedGoomba deleted\n");
}

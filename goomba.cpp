#include "goomba.h"
#include <iostream>

Goomba::Goomba(int x, int y) : Monster(x, y)
{
    this->setLife(1);
    this->moveLSprite = QPixmap(":images/goomba.png");
    this->rect = QRect(x, y, 50, moveLSprite.size().height());
}

Goomba::~Goomba()
{
    std::cout << ("Goomba deleted\n");
}

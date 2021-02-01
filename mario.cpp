#include "mario.h"
#include <iostream>

Mario::Mario(int x, int y ) : Person(x,y)
{
    this->moveRSprite = QPixmap(":images/mario_walk_sprites.png");
    this->moveLSprite = QPixmap(":images/mario_walk_sprites_left.png");
    this->stopSprite = QPixmap(":images/mario_stop.png");
    this->rect = QRect(x, y, 60, moveRSprite.size().height());
    this->dieRect = QRect(x, y, 60, stopSprite.size().height());
}

Mario::~Mario()
{
    std::cout << ("Mario deleted\n");
}

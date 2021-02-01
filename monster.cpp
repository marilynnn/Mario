#include "monster.h"
#include <iostream>
#include <QDebug>
#include <QRect>

Monster::Monster(int x, int y)
{
    dead = false;
}

Monster::~Monster()
{
    std::cout << ("Monster deleted\n");
}

bool Monster::isDead()
{
    return dead;
}

void Monster::setDead(bool destr)
{
    dead = destr;
}

void Monster::move(int x ,int y)
{
    rect.moveTo(x, y);
}

void Monster::moveDie(int x ,int y)
{
    dieRect.moveTo(x, y);
}

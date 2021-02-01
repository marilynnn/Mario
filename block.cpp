#include "block.h"
#include <iostream>
#include <QDebug>

Block::Block(int x, int y, QString picture)
{
    image.load(picture);
    destroyed = false;
    rect = image.rect();
    rect.translate(x, y);
}

Block::~Block()
{
    std::cout << ("Block deleted\n");
}

QRect Block::getRect()
{
    return rect;
}

void Block::setRect(QRect rct)
{
    rect = rct;
}

QImage & Block::getImage()
{
    return image;
}

bool Block::isDestroyed()
{
    return destroyed;
}

void Block::setDestroyed(bool destr)
{
    destroyed = destr;
}

void Block::moveBlock(int x)
{
    rect.moveTo(x, rect.top());
}

void Block::move(int x ,int y)
{
    rect.moveTo(x, y);
}

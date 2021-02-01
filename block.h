#ifndef BLOCK_H
#define BLOCK_H

#include <QImage>
#include <QRect>
#include <QString>

class Block
{
  public:
    Block(int, int , QString );
    ~Block();
    bool isDestroyed();
    void setDestroyed(bool);
    QRect getRect();
    void setRect(QRect);
    QImage & getImage();
    void move(int ,int );
    void moveBlock(int );

  private:
    QImage image;
    QRect rect;
    bool destroyed;
};

#endif // BLOCK_H

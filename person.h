#ifndef PERSON_H
#define PERSON_H

#include <QPixmap>
#include <QImage>
#include <QRect>
#include <QString>

class Person
{
  public:
    Person(int, int);
    ~Person();
    bool isDead();
      void setDead(bool);
      QRect getRect(){ return rect; }
      void setRect(QRect rect){ this->rect = rect; }
      QRect getDieRect(){ return dieRect; }
      void setDieRect(QRect dieRect){ this->dieRect = dieRect; }
      QImage & getImage(){ return image; }
      QPixmap getMoveRSprite(){ return moveRSprite; }
      QPixmap getMoveLSprite(){ return moveLSprite; }
      QPixmap getStopSprite(){ return stopSprite; }
    void move(int , int);
    void moveDie(int ,int );
      int getLife(){ return life; }
      void setLife(int life){ this->life = life; }

protected:
    QPixmap moveRSprite;
    QPixmap moveLSprite;
    QPixmap stopSprite;
    QPixmap jumpSprite;
    QRect rect;
    QRect dieRect;

 private:
    QImage image;
    QPixmap imageMap;
    bool dead;
    int life = 3;
};

#endif // PERSON_H

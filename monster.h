#ifndef MONSTER_H
#define MONSTER_H

#include <QPixmap>
#include <QImage>
#include <QRect>
#include <QString>

class Monster
{
  public:
    Monster(int, int);
    ~Monster();

    bool isDead();
      void setDead(bool);
      QRect getRect(){ return rect; }
      void setRect(QRect rect){ this->rect = rect; }
      QRect getDieRect(){ return dieRect; }
      void setDieRect(QRect dieRect){ this->dieRect = dieRect; }
      QImage & getImage(){ return image; }
      QPixmap getMoveLSprite(){ return moveLSprite; }
    void move(int , int);
    void moveDie(int ,int );
      int getLife(){ return life; }
      void setLife(int life){ this->life = life; }

  protected:
    QPixmap moveLSprite;
    QRect rect;
    QRect dieRect;

   private:
    QImage image;
    QPixmap imageMap;
    bool dead;
    int life = 1;
};

#endif // MONSTER_H

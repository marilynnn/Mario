#include "model.h"
#include "goomba.h"
#include <mario.h>
#include <spike.h>
#include <flag.h>
#include <floor.h>
#include <flag.h>
#include <splashscreen.h>
#include <gameover.h>
#include <QDebug>

Model::Model()
{
    this->floors = new QMap<int,Floor *>;
    this->castle = new QMap<int,Castle*>;
    this->spikes= new QMultiMap<int,Spike*>;
    this->flags= new QMap<int,Flag*>;
    this->mario = new Mario(100,144);
    this->goomba = new Goomba(-50, 0);
    this->wingedGoomba = new WingedGoomba(-50, 0);
    this->splashScreen = new SplashScreen(0, 0);
    this->gameOver = new GameOver(0, 0);
    this->won = new Won(0, 0);
    this->background = new QMap<int,Background *>;

    floorCount=0;

    for (int i=0; i<2; i++)
  {
      Background* b = new Background(i*800, 0);
      background->insert(backgroundCount, b);
      backgroundCount++;
  }
  for (int i=0; i<13; i++)
  {
      for (int j=0; j<1; j++)
      {
          Floor* k = new Floor(i*blockSize,500-blockSize-j*blockSize);
          floors->insert(floorCount,k);
          floorCount++;
      }
  }
}

Model::~Model()
{
    QMap< int,Floor *>::const_iterator i = floors->constBegin();
    QMap<int, Castle*>::const_iterator c = castle->constBegin();
    QMultiMap< int,Spike*>::const_iterator s = spikes->constBegin();
    QMap< int,Flag*>::const_iterator f = flags->constBegin();

    while (i != floors->constEnd())
    {
        delete i.value();
        ++i;
    }

    while (c != castle->constEnd())
    {
        delete c.value();
        ++c;
    }
    while (s != spikes->constEnd())
    {
        delete  s.value();
        ++s;
    }
    while (f != flags->constEnd())
    {
        delete  f.value();
        ++f;
    }

    floors->clear();
    delete floors;
    spikes->clear();
    delete spikes;
    flags->clear();
    delete flags;
    castle->clear();
    delete castle;

    delete mario;
    delete splashScreen;
    delete gameOver;
    delete won;
    delete wingedGoomba;
    if(goomba != NULL){ delete goomba; }
}

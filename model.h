#ifndef MODEL_H
#define MODEL_H

#include <QMap>
#include <QMutableMapIterator>
#include <QString>
#include <splashscreen.h>
#include <gameover.h>
#include <won.h>
#include "background.h"
#include "castle.h"
#include "goomba.h"
#include "wingedgoomba.h"

class Spike;
class Flag;
class Floor;
class Mario;
class Castle;
class Goomba;
class WingedGoomba;

class Model
{
  public:
      Model();
      ~Model();
        Mario *getMario(){ return mario; }
        Goomba *getGoomba(){ return goomba; }
        WingedGoomba *getWingedGoomba(){ return wingedGoomba; }

      SplashScreen *getSplashScreen(){ return splashScreen; }
      GameOver *getGameOver(){ return gameOver; }
      Won *getWon(){ return won; }
        QMultiMap<int, Spike*> *getSpikes(){ return spikes; }
        QMap<int, Flag*> *getFlags(){ return flags; }
        QMap<int, Floor*> *getFloors(){ return floors; }
        QMap<int, Background*> *getBackground(){ return background; }

        QMap<int, Castle*> *getCastle(){return castle;}
        int getFloorCount(){ return floorCount; }
        void setFloorCount(){ this->floorCount++; }
        int getSpikeCount(){ return spikeCount; }
        void setSpikeCount(){ this->spikeCount++; }
        int getFlagCount(){ return flagCount; }
        void setFlagCount(){ this->flagCount++; }
        int getBackgroundCount(){ return backgroundCount; }
        void setBackgroundCount(){ this->backgroundCount++; }
        int getCastleCount(){return castleCount;}
        void setCastleCount(){this->castleCount++;}

        void setMario(Mario *mario){ this->mario = mario; }
        void setGoomba(Goomba *goomba){ this->goomba = goomba; }
        void setWingedGoomba(WingedGoomba *wingedGoomba) { this->wingedGoomba = wingedGoomba; }
        void setSpikes(QMultiMap<int, Spike*> *spikes){this->spikes = spikes;}
        void setFloors(QMap<int,Floor*> *floors){ this->floors = floors; }
        void setFlag(QMap<int, Flag*> *flag){this->flags = flag;}
        void setCastle(QMap<int, Castle*> *castle){this->castle = castle;}
      int const blockSize = 50;
      void deleteGoomba() { delete this->goomba; }
      void deleteWingedGoomba() { delete this->wingedGoomba; }

  private:
      QMap<int, Floor*> *floors;
      QMap<int, Castle*> *castle;
      QMultiMap<int, Spike*> *spikes;
      QMap<int, Flag*> *flags;
      Mario *mario;
      Goomba *goomba;
      WingedGoomba *wingedGoomba;


      SplashScreen *splashScreen;
      GameOver *gameOver;
      Won *won;
      QMap<int, Background*> *background;
      int floorCount;

      int spikeCount;
      int backgroundCount;

      int flagCount;
      int castleCount;
};
#endif // MODEL_H

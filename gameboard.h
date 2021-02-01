#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QKeyEvent>
#include <QMutableMapIterator>
#include <QElapsedTimer>


#include "spike.h"
#include "flag.h"
#include "floor.h"
#include "mario.h"

class Model;
class View;

class GameBoard : public QWidget
{
    Q_OBJECT

public:
    GameBoard(Model *model, QWidget *parent = 0);
    ~GameBoard();
      int getIterBackground(){ return this->iterBackground; }
      void setIterBackground(int iterBackground){ this->iterBackground = iterBackground; }

  protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void stopGame();
    void gameOver();
    void gameWon();
    void removeDestroyed();
    void movementMario();
    void movementMapLeft();
    void movementMapRight();
    void movementBackground();
    bool intersect();
    void splashScreen();
    void generateMap();
    void movementGoomba();
    void movementWingedGoomba();

private:
    Model *model;
    int timerId;
    bool gameStarted;

    int xRelatif;
    int yRelatif;
    int spikeCount;
    int flagCount;
    int moveCount;
    int floorCount;
    bool moveR;
    bool moveL;
    bool isJumping;
    bool isSplashScreen;
    bool isGameOver;
    bool isGoombakilled;
    bool isWon;
    int iterBackground;
    float currentFrame = 1;
    int tempMove = 0;
    char* lastMove = "R";

    qreal opacity = 1;
    QElapsedTimer gameTimer;

};
#endif // GAMEBOARD_H

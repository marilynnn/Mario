#include "gameboard.h"
#include <QPainter>
#include <QApplication>
#include <QDebug>
#include "model.h"
#include <iostream>
#include <QMediaPlayer>

GameBoard::GameBoard(Model *model, QWidget *parent)
: model(model), QWidget(parent)
{
  //Start timer of one Game
  gameTimer.start();

  this->model = model;
  timerId = startTimer(10);
  gameStarted = true;
  moveL=false;
  moveR=false;
  isJumping=false;
  isGoombakilled=false;
  xRelatif = -100;
  yRelatif = 0;
  spikeCount=0;
  flagCount=0;
  moveCount=0;
  isSplashScreen  = true;
  setIterBackground(0);
  isGameOver = false;
  isWon= false;

  // Music when game is launched
  QMediaPlayer * musicBG = new QMediaPlayer();
  musicBG->setMedia(QUrl("qrc:/sounds/music.mp3"));
  musicBG->setVolume(5);
  musicBG->play();
}

GameBoard::~GameBoard()
{
  killTimer(timerId);
}

void GameBoard::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);

  QMap< int,Background *>::const_iterator b = model->getBackground()->constBegin();
  while (b != model->getBackground()->constEnd())
  {
    painter.drawImage(b.value()->getRect(),b.value()->getImage());
    ++b;
  }

  QMap< int,Floor *>::const_iterator i = model->getFloors()->constBegin();
  QMap< int,Castle*>::const_iterator c = model->getCastle()->constBegin();
  QMap< int,Spike*>::const_iterator s = model->getSpikes()->constBegin();
  QMap< int,Flag*>::const_iterator f = model->getFlags()->constBegin();

  // Generate Floor
  while (i != model->getFloors()->constEnd())
  {
    painter.drawImage(i.value()->getRect(),i.value()->getImage());
    ++i;
  }

  // Generate Spikes
  while (s != model->getSpikes()->constEnd())
  {
    painter.drawImage(s.value()->getRect(),s.value()->getImage());
    ++s;
  }


  // Generate Castle
  while (c != model->getCastle()->constEnd())
  {
    painter.drawImage(c.value()->getRect(),c.value()->getImage());
    ++c;
  }

  // Generate Flag
  while (f != model->getFlags()->constEnd())
  {
    painter.drawImage(f.value()->getRect(),f.value()->getImage());
    ++f;
  }

  // Generate Mario (walking right, left, and stopping)
  QRect sourceRect = QRect(currentFrame, 1, 57, 68);
  if(moveR)
  {
    painter.drawPixmap(model->getMario()->getRect(), model->getMario()->getMoveRSprite(), sourceRect);
  }
  else if(moveL)
  {
    painter.drawPixmap(model->getMario()->getRect(), model->getMario()->getMoveLSprite(), sourceRect);
  }
  else
  {
    painter.drawPixmap(model->getMario()->getRect(), model->getMario()->getStopSprite(), sourceRect);
  }

  // Generate Goomba
  if( (model->getGoomba()!= NULL) && (model->getGoomba()->getLife() != 0) )
  {
    QRect sourceRectGoomba = QRect(currentFrame, 1, 57, 68);
    painter.drawPixmap(model->getGoomba()->getRect(), model->getGoomba()->getMoveLSprite(), sourceRectGoomba);
  }

  // Generate WingedGoomba
  QRect sourceRectWingedGoomba = QRect(currentFrame, 1, 57, 68);
  painter.drawPixmap(model->getWingedGoomba()->getRect(), model->getWingedGoomba()->getMoveLSprite(), sourceRectWingedGoomba);

  // Generate SplashScreen (fading)
  for(int i = 0 ; i < model->getMario()->getLife() ; i++)
  if(isSplashScreen)
  {

    opacity = opacity - 0.0004;
    painter.setOpacity(opacity);
    painter.drawImage(model->getSplashScreen()->getRect(), model->getSplashScreen()->getImage());
  }
  else
  {
    opacity = 1;
    painter.setOpacity(opacity);
  }
  // Generate GameOverScreen
  if(isGameOver)
  {
    opacity = 1;
    painter.setOpacity(opacity);
    painter.drawImage(model->getGameOver()->getRect(), model->getGameOver()->getImage());
  }
  // Generate WonScreen
  if(isWon)
  {
    opacity = 1;
    painter.setOpacity(opacity);
    painter.drawImage(model->getWon()->getRect(), model->getWon()->getImage());
  }

}

// Game loop
void GameBoard::timerEvent(QTimerEvent *event)
{
  splashScreen();
  movementMario();
  movementGoomba();
  movementWingedGoomba();
  removeDestroyed();
  repaint();
}

// controlling
void GameBoard::keyPressEvent(QKeyEvent *event)
{
  if(event->key() == Qt::Key_Right || event->key() == Qt::Key_D)
  {
    moveR=true;
  }
  else if(event->key() == Qt::Key_Left || event->key() == Qt::Key_A)
  {
    moveL=true;
  }
  else if(event->key() == Qt::Key_Space && intersect())
  {
    isJumping=true;
  }
  else if (event->key() == Qt::Key_Escape)
  {
    stopGame();
    qApp->exit();
  }
  else
  {
    event->ignore();
  }
}

// Action listenner on keystrokes (releasing)
void GameBoard::keyReleaseEvent(QKeyEvent *event)
{
  if(event->key() == Qt::Key_Right || event->key() == Qt::Key_D)
  {
    moveR=false;
  }
  else if(event->key() == Qt::Key_Left || event->key() == Qt::Key_A)
  {
    moveL=false;
  }
  else
  {
    event->ignore();
  }
}

// Exit game
void GameBoard::stopGame()
{
  killTimer(timerId);
  gameStarted = false;
}

// Manage SplashScreen
void GameBoard::splashScreen()
{
  int x=model->getSplashScreen()->getRect().x();
  int y=model->getSplashScreen()->getRect().y();
  y = y - 0.5;

  if(model->getSplashScreen()->getRect().bottom() > 0 && isSplashScreen)
  {
    model->getSplashScreen()->move(x, y);
  }
  else
  {
    isSplashScreen = false;
  }
}

// Manage GameOverScreen
void GameBoard::gameOver()
{
  isGameOver = true;

  // Get the time of one game in seconds
  int gameTime = gameTimer.elapsed()/1000;

  qDebug() << "You lose. Duration of the game:  " << gameTime << "seconds";
  qDebug() << "Press esc to exit.";
  stopGame();
}

// Manage WonScreen
void GameBoard::gameWon()
{
  isWon = true;

  // Get the time of one game in seconds
  int gameTime = gameTimer.elapsed()/1000;
  qDebug() << "You won. Duration of the game:  " << gameTime << "seconds";
  qDebug() << "Press esc to exit.";
  stopGame();
}

// Garbage Collector
void GameBoard::removeDestroyed()
{
  QMutableMapIterator<int ,Floor * > i(*model->getFloors());
  while (i.hasNext())
  {
    i.next();
    if (i.value()->isDestroyed())
    {
      qDebug() << "Remove Floor:" << i.key() ;
      i.remove();
    }
  }
  QMutableMapIterator<int ,Spike* > s(*model->getSpikes());
  while (s.hasNext())
  {
    s.next();
    if (s.value()->isDestroyed())
    {
      qDebug() << "Remove Spikes:" << s.key() ;
      s.remove();
    }
  }
  QMutableMapIterator<int ,Flag* > f(*model->getFlags());
  while (f.hasNext())
  {
    f.next();
    if (f.value()->isDestroyed())
    {
      qDebug() << "Remove Flags:" << f.key() ;
      f.remove();
    }
  }
  QMutableMapIterator<int ,Background * > b(*model->getBackground());
  while (b.hasNext())
  {
    b.next();
    if (b.value()->isDestroyed())
    {
      qDebug() << "Remove Background:" << b.key() ;
      b.remove();
    }
  }
}

// Manage Mario
void GameBoard::movementMario()
{
  int y=model->getMario()->getRect().y();
  int x=model->getMario()->getRect().x();
  int checkGoomba = 0;

  // While jumping
  if(isJumping)
  {
    xRelatif+=3;
    yRelatif=(-0.02*(xRelatif*xRelatif)+100);
    y = 300-yRelatif;

    // Kill the Goomba if you jump on it
    if(model->getGoomba()->getRect().intersects(model->getMario()->getRect()))
    {
      qDebug() << "Goomba Dead";
      isGoombakilled=true;
      model->getGoomba()->setLife(0);
      model->deleteGoomba();
    }

    // Win if you hit the flag
    if(!model->getFlags()->empty() && model->getFlags()->constBegin().value()->getRect().intersects(model->getMario()->getRect()))
    {
      gameWon();
    }

    // Die if you hit the Winged Goomba
    if(model->getWingedGoomba()->getRect().intersects(model->getMario()->getRect()))
    {
      qDebug() << "Kill by the WingedGoomba, SHAME again.";
      gameOver();
    }

    // Die if you hit the spikes
    if(!model->getSpikes()->empty())
    {
      QList<Spike *> list = model->getSpikes()->values(1);
      for(int i=0; i<list.size(); i++)
      {
        if(model->getMario()->getRect().intersects(list[i]->getRect()))
        gameOver();
      }
    }

    // Die if you fall off
    if(model->getMario()->getRect().y()>=500)
    {
      gameOver();
    }

    // Jumping left when reaching left border
    if(moveL && model->getMario()->getRect().x()<=20)
    {
      movementMapLeft();
      moveCount--;
    }
    // Jumping left
    else if(moveL && model->getMario()->getRect().x()>=20)
    {
      x-=2;
      moveCount--;
    }
    // Jumping right
    else if(moveR && model->getMario()->getRect().x()<=150)
    {
      x+=2;
      moveCount++;
    }
    // Jumping right when reaching right border
    else if(moveR && model->getMario()->getRect().x()>=150)
    {
      movementMapRight();
      moveCount++;
    }
    currentFrame = 0;
    model->getMario()->move(x,y);
  }

  // While walking
  if(intersect())
  {
    xRelatif=-100;
    yRelatif=0;
    isJumping=false;

    // Die if you hit the Goomba
    if(model->getGoomba()->getRect().intersects(model->getMario()->getRect()))
    {
      gameOver();
    }

    // Win if you hit the flag
    if(!model->getFlags()->empty() && model->getFlags()->constBegin().value()->getRect().intersects(model->getMario()->getRect()))
    {
      gameWon();
    }

    // Die if you hit the spikes
    if(!model->getSpikes()->empty())
    {
      QList<Spike *> list = model->getSpikes()->values(1);
      for(int i=0; i<list.size(); i++)
      {
        if(model->getMario()->getRect().intersects(list[i]->getRect()))
        {
          gameOver();
        }
      }
    }


    // Die if you fall off
    if(model->getMario()->getRect().y()>=500)
    {
      gameOver();
    }

    // Walking left when reaching left border
    if(moveL && model->getMario()->getRect().x()<=20){
      movementMapLeft();
      moveCount--;
    }
    // Walking left
    else if(moveL && model->getMario()->getRect().x()>=20){
      x-=2;
      moveCount--;
    }
    // Walking right
    else if(moveR && model->getMario()->getRect().x()<=150){
      x+=2;
      moveCount++;
    }
    // Walking right when reaching right border
    else if(moveR && model->getMario()->getRect().x()>=150){
      movementMapRight();
      moveCount++;
    }
    model->getMario()->move(x, y);

    // Animating to make Mario walk
    if(moveR && tempMove == 1){
      currentFrame += 60;
      if (currentFrame >= 180 )
      currentFrame = 1;
      tempMove = 0;
    }
    else if(moveR)
    {
      tempMove++;
    }
    else if(moveL && tempMove == 1)
    {
      currentFrame -= 60;
      if (currentFrame <= 0 )
      currentFrame = 179;
      tempMove = 0;
    }
    else if(moveL)
    {
      tempMove++;
    }
    else
    {
      currentFrame = 0;
    }
  }

  // While falling after spawn
  if(!intersect() && !isJumping)
  {
    y += 4;
    // Win if you hit the flag
    if(!model->getFlags()->empty() && model->getFlags()->constBegin().value()->getRect().intersects(model->getMario()->getRect()))
    {
      gameWon();
    }

    // Die if you hit the spikes
    if(!model->getSpikes()->empty() && model->getSpikes()->constBegin().value()->getRect().intersects(model->getMario()->getRect()))
    {
      gameOver();
    }

    // Die if you fall off
    if(model->getMario()->getRect().y()>=500)
    {
      gameOver();
    }

    // Going left when reaching left border
    if(moveL && model->getMario()->getRect().x()<=20)
    {
      movementMapLeft();
      moveCount--;
    }
    // Going left
    else if(moveL && model->getMario()->getRect().x()>=20)
    {
      x-=3;
      moveCount--;
    }
    // Going right
    else if(moveR && model->getMario()->getRect().x()<=150)
    {
      x+=3;
      moveCount++;
    }
    // Going right when reaching right border
    else if(moveR && model->getMario()->getRect().x()>=150)
    {
      movementMapRight();
      moveCount++;
    }
    model->getMario()->move(x, y);
  }
}

// Mario walking (intersecting with the floor)
bool GameBoard::intersect()
{
  QMap< int,Floor *>::const_iterator i = model->getFloors()->constBegin();
  model->getFloors();
  while (i != model->getFloors()->constEnd())
  {
    if ((model->getMario()->getRect()).intersects(i.value()->getRect()))
    {
      return true;
    }
    ++i;
  }
  return false;
}

// Goomba walking left
void GameBoard::movementGoomba()
{
  if (!isGoombakilled)
  {
    int y=model->getGoomba()->getRect().y();
    int x=model->getGoomba()->getRect().x();
    model->getGoomba()->move(x-1.25, y);
  }
  else
  {
    model->getGoomba()->move(0,0);
  }
}

// WingedGoomba flying left
void GameBoard::movementWingedGoomba()
{
  int x=model->getWingedGoomba()->getRect().x();
  int y=model->getWingedGoomba()->getRect().y();

  model->getWingedGoomba()->move(x-1.25, y);
}

// Map scrolling when going to the right
void GameBoard::movementMapRight()
{
  int x0=0;
  int y0=0;

  // Ennemies not affected by the scrolling
  movementGoomba();
  movementWingedGoomba();

  // Background scrolling (slowly)
  QMap< int,Background *>::const_iterator k = model->getBackground()->constBegin();
  if(getIterBackground() == 4)
  {
    while (k != model->getBackground()->constEnd())
    {
      x0=k.value()->getRect().x();
      k.value()->moveBlock(x0-1);
      ++k;
    }
    setIterBackground(0);
  }
  else
  {
    while (k != model->getBackground()->constEnd())
    {
      x0=k.value()->getRect().x();
      k.value()->moveBlock(x0);
      ++k;
    }
    setIterBackground(getIterBackground() + 1);
  }

  // Floor scrolling
  QMap< int,Floor *>::const_iterator i = model->getFloors()->constBegin();
  while (i != model->getFloors()->constEnd())
  {
    x0=i.value()->getRect().x();
    i.value()->moveBlock(x0-4);
    ++i;
  }


  // Spikes scrolling
  QMap< int,Spike*>::const_iterator s = model->getSpikes()->constBegin();
  while (s != model->getSpikes()->constEnd())
  {
    x0=s.value()->getRect().x();
    s.value()->moveBlock(x0-4);
    ++s;
  }

  // Flag scrolling
  QMap< int,Flag*>::const_iterator f = model->getFlags()->constBegin();
  while (f != model->getFlags()->constEnd())
  {
    x0=f.value()->getRect().x();
    f.value()->moveBlock(x0-4);
    ++f;
  }

  // Castle scrolling
  QMap< int,Castle *>::const_iterator c = model->getCastle()->constBegin();
  while (c != model->getCastle()->constEnd())
  {
    x0=c.value()->getRect().x();
    c.value()->moveBlock(x0-4);
    ++c;
  }

  // Endless loop to generate floor
  QMap< int,Floor *>::const_iterator i0= model->getFloors()->constBegin();
  while (i0 != model->getFloors()->constEnd())
  {
    if(i0.value()->getRect().x() < -model->blockSize)
    {
      i0.value()->setDestroyed(true);
      x0=i0.value()->getRect().x();
      y0=i0.value()->getRect().y();
      Floor* k =new Floor(x0+13*50,y0);
      model->getFloors()->insert(model->getFloorCount(),k);
      qDebug() << "create Floor:" << model->getFloorCount() ;
      model->setFloorCount();
    }
    ++i0;
  }

  // Endless loop to generate background
  QMap< int,Background *>::const_iterator b0= model->getBackground()->constBegin();
  while (b0 != model->getBackground()->constEnd())
  {
    if(b0.value()->getRect().x() < - b0.value()->getRect().width() + 2)
    {
      b0.value()->setDestroyed(true);
      Background* b = new Background(b0.value()->getRect().width(),0);
      model->getBackground()->insert(model->getBackgroundCount(), b);
      qDebug() << "create Background:" << model->getBackgroundCount();
      model->setBackgroundCount();
    }
    ++b0;
  }

  // After a little walk, generate the level
  if(moveCount==100)
  {
    generateMap();
  }
}

// Map scrolling when going to the left
void GameBoard::movementMapLeft()
{
  int x0=0;
  int y0=0;

  // Background scrolling (slowly)
  QMap< int,Background *>::const_iterator k = model->getBackground()->constBegin();
  if(getIterBackground() == 4){
    while (k != model->getBackground()->constEnd())
    {
      x0=k.value()->getRect().x();
      k.value()->moveBlock(x0+1);
      ++k;
    }
    setIterBackground(0);
  }
  else{
    while (k != model->getBackground()->constEnd())
    {
      x0=k.value()->getRect().x();
      k.value()->moveBlock(x0);
      ++k;
    }
    setIterBackground(getIterBackground() + 1);
  }

  // Floor scrolling
  QMap< int,Floor *>::const_iterator i = model->getFloors()->constBegin();
  while (i != model->getFloors()->constEnd())
  {
    x0=i.value()->getRect().x();
    i.value()->moveBlock(x0+4);
    ++i;
  }


  // Spikes scrolling
  QMap< int,Spike*>::const_iterator s = model->getSpikes()->constBegin();
  while (s != model->getSpikes()->constEnd())
  {
    x0=s.value()->getRect().x();
    s.value()->moveBlock(x0+4);
    ++s;
  }

  // Flag scrolling
  QMap< int,Flag*>::const_iterator f = model->getFlags()->constBegin();
  while (f != model->getFlags()->constEnd())
  {
    x0=f.value()->getRect().x();
    f.value()->moveBlock(x0+4);
    ++f;
  }

  // Castle scrolling
  QMap< int,Castle *>::const_iterator c = model->getCastle()->constBegin();
  while (c != model->getCastle()->constEnd())
  {
    x0=c.value()->getRect().x();
    c.value()->moveBlock(x0+4);
    ++c;
  }
}

// Level bulding (obstacles, monsters, flag, castle)
void GameBoard::generateMap()
{
  // Goomba
  Goomba *g2 = new Goomba(551, 415);
  model->setGoomba(g2);

  // WingedGoomba
  WingedGoomba *wg1 = new WingedGoomba(1250, 200);
  model->setWingedGoomba(wg1);

 // line of 3 spikes
  int x0=600;
  int y0=250;
  Spike* spike =new Spike(x0,y0);
  model->getSpikes()->insert(1, spike);
  qDebug() << "create Spike:" << model->getSpikeCount() ;
  model->setSpikeCount();

  x0+=50;
  y0=250;
  Spike* spike2 =new Spike(x0,y0);
  model->getSpikes()->insert(1, spike2);
  qDebug() << "create Spike:" << model->getSpikeCount() ;
  model->setSpikeCount();

  x0+=50;
  y0=250;
  Spike* spike3 =new Spike(x0,y0);
  model->getSpikes()->insert(1, spike3);
  qDebug() << "create Spike:" << model->getSpikeCount() ;
  model->setSpikeCount();

  // tower of 2 spikes
  x0=850;
  y0=400;
  Spike* spike4 =new Spike(x0,y0);
  model->getSpikes()->insert(1, spike4);
  qDebug() << "create Spike:" << model->getSpikeCount() ;
  model->setSpikeCount();

  y0=350;
  Spike* spike5 =new Spike(x0,y0);
  model->getSpikes()->insert(1, spike5);
  qDebug() << "create Spike:" << model->getSpikeCount() ;
  model->setSpikeCount();

  // line of 3 spikes
  x0=1050;
  y0=250;
  Spike* spike6 =new Spike(x0,y0);
  model->getSpikes()->insert(1, spike6);
  qDebug() << "create Spike:" << model->getSpikeCount() ;
  model->setSpikeCount();

  x0+=50;
  y0=250;
  Spike* spike7 =new Spike(x0,y0);
  model->getSpikes()->insert(1, spike7);
  qDebug() << "create Spike:" << model->getSpikeCount() ;
  model->setSpikeCount();

  x0+=50;
  y0=250;
  Spike* spike8 =new Spike(x0,y0);
  model->getSpikes()->insert(1, spike8);
  qDebug() << "create Spike:" << model->getSpikeCount() ;
  model->setSpikeCount();

  // reversed L shape
  x0=1300;
  y0=400;
  Spike* spike9 =new Spike(x0,y0);
  model->getSpikes()->insert(1, spike9);
  qDebug() << "create Spike:" << model->getSpikeCount() ;
  model->setSpikeCount();

  x0=1350;
  y0=400;
  Spike* spike10 =new Spike(x0,y0);
  model->getSpikes()->insert(1, spike10);
  qDebug() << "create Spike:" << model->getSpikeCount() ;
  model->setSpikeCount();

  y0=350;
  Spike* spike11 =new Spike(x0,y0);
  model->getSpikes()->insert(1, spike11);
  qDebug() << "create Spike:" << model->getSpikeCount() ;
  model->setSpikeCount();

  // V shape
  x0=1500;
  y0=250;
  Spike* spike12 =new Spike(x0,y0);
  model->getSpikes()->insert(1, spike12);
  qDebug() << "create Spike:" << model->getSpikeCount() ;
  model->setSpikeCount();

  x0+=50;
  y0=300;
  Spike* spike13 =new Spike(x0,y0);
  model->getSpikes()->insert(1, spike13);
  qDebug() << "create Spike:" << model->getSpikeCount() ;
  model->setSpikeCount();

  x0+=50;
  y0=250;
  Spike* spike14 =new Spike(x0,y0);
  model->getSpikes()->insert(1, spike14);
  qDebug() << "create Spike:" << model->getSpikeCount() ;
  model->setSpikeCount();

  // tower of 2 spikes
  x0=1750;
  y0=350;
  Spike* spike15 =new Spike(x0,y0);
  model->getSpikes()->insert(1, spike15);
  qDebug() << "create Spike:" << model->getSpikeCount() ;
  model->setSpikeCount();

  y0=400;
  Spike* spike16 =new Spike(x0,y0);
  model->getSpikes()->insert(1, spike16);
  qDebug() << "create Spike:" << model->getSpikeCount() ;
  model->setSpikeCount();

  // tower of 3 spikes
  x0=1900;
  y0=300;
  Spike* spike17 =new Spike(x0,y0);
  model->getSpikes()->insert(1, spike17);
  qDebug() << "create Spike:" << model->getSpikeCount() ;
  model->setSpikeCount();

  y0=350;
  Spike* spike18 =new Spike(x0,y0);
  model->getSpikes()->insert(1, spike18);
  qDebug() << "create Spike:" << model->getSpikeCount() ;
  model->setSpikeCount();

  y0=400;
  Spike* spike19 =new Spike(x0,y0);
  model->getSpikes()->insert(1, spike19);
  qDebug() << "create Spike:" << model->getSpikeCount() ;
  model->setSpikeCount();


  // bracket shaped + dot
  x0=2200;
  y0=180;
  Spike* spike20 = new Spike(x0, y0);
  model->getSpikes()->insert(1, spike20);
  qDebug() << "create Spike:" << model->getSpikeCount();
  model->setSpikeCount();

  x0+=50;
  y0=130;
  Spike* spike21 = new Spike(x0, y0);
  model->getSpikes()->insert(1, spike21);
  qDebug() << "create Spike:" << model->getSpikeCount();
  model->setSpikeCount();

  x0+=50;
  y0=130;
  Spike* spike22 = new Spike(x0, y0);
  model->getSpikes()->insert(1, spike22);
  qDebug() << "create Spike:" << model->getSpikeCount();
  model->setSpikeCount();

  y0=400;
  Spike* spike23 = new Spike(x0, y0);
  model->getSpikes()->insert(1, spike23);
  qDebug() << "create Spike:" << model->getSpikeCount();
  model->setSpikeCount();

  x0+=50;
  y0=130;
  Spike* spike24 = new Spike(x0, y0);
  model->getSpikes()->insert(1, spike24);
  qDebug() << "create Spike:" << model->getSpikeCount();
  model->setSpikeCount();

  x0+=50;
  y0=180;
  Spike* spike25 = new Spike(x0, y0);
  model->getSpikes()->insert(1, spike25);
  qDebug() << "create Spike:" << model->getSpikeCount();
  model->setSpikeCount();

  // diagonal of 2 spikes
  x0=2550;
  y0=280;
  Spike* spike26 = new Spike(x0, y0);
  model->getSpikes()->insert(1, spike26);
  qDebug() << "create Spike:" << model->getSpikeCount();
  model->setSpikeCount();

  x0+=50;
  y0=330;
  Spike* spike27 = new Spike(x0, y0);
  model->getSpikes()->insert(1, spike27);
  qDebug() << "create Spike:" << model->getSpikeCount();
  model->setSpikeCount();

  // 2 dots
  x0=2750;
  y0=400;
  Spike* spike28 = new Spike(x0, y0);
  model->getSpikes()->insert(1, spike28);
  qDebug() << "create Spike:" << model->getSpikeCount();
  model->setSpikeCount();

  x0+=150;
  y0=400;
  Spike* spike29 = new Spike(x0, y0);
  model->getSpikes()->insert(1, spike29);
  qDebug() << "create Spike:" << model->getSpikeCount();
  model->setSpikeCount();

  // fence
  x0=3150;
  y0=400;
  Spike* spike30 = new Spike(x0, y0);
  model->getSpikes()->insert(1, spike30);
  qDebug() << "create Spike:" << model->getSpikeCount();
  model->setSpikeCount();

  y0=350;
  Spike* spike31 = new Spike(x0, y0);
  model->getSpikes()->insert(1, spike31);
  qDebug() << "create Spike:" << model->getSpikeCount();
  model->setSpikeCount();

  y0=100;
  Spike* spike32 = new Spike(x0, y0);
  model->getSpikes()->insert(1, spike32);
  qDebug() << "create Spike:" << model->getSpikeCount();
  model->setSpikeCount();

  y0=50;
  Spike* spike33 = new Spike(x0, y0);
  model->getSpikes()->insert(1, spike33);
  qDebug() << "create Spike:" << model->getSpikeCount();
  model->setSpikeCount();

  y0=0;
  Spike* spike34 = new Spike(x0, y0);
  model->getSpikes()->insert(1, spike34);
  qDebug() << "create Spike:" << model->getSpikeCount();
  model->setSpikeCount();

  // dots
  x0=3350;
  y0=400;
  Spike* spike35 = new Spike(x0, y0);
  model->getSpikes()->insert(1, spike35);
  qDebug() << "create Spike:" << model->getSpikeCount();
  model->setSpikeCount();

  x0+=150;
  y0=400;
  Spike* spike36 = new Spike(x0, y0);
  model->getSpikes()->insert(1, spike36);
  qDebug() << "create Spike:" << model->getSpikeCount();
  model->setSpikeCount();

  x0+=150;
  y0=400;
  Spike* spike37 = new Spike(x0, y0);
  model->getSpikes()->insert(1, spike37);
  qDebug() << "create Spike:" << model->getSpikeCount();
  model->setSpikeCount();

  // flag
  x0=4000;
  y0=190;
  Flag* flag =new Flag(x0,y0);
  model->getFlags()->insert(model->getFlagCount(), flag);
  qDebug() << "create Flag:" << model->getFlagCount() ;
  model->setFlagCount();

  // castle
  x0 += 150;
  y0 = 118;
  Castle* castle = new Castle(x0, y0);
  model->getCastle()->insert(model->getCastleCount(), castle);
  qDebug() << "create Castle: " << model->getCastleCount();
  model->setCastleCount();
}

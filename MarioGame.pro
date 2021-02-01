

QT       += core gui \
        multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mario
TEMPLATE = app
QMAKE_CXXFLAGS+= -std=c++11
QMAKE_LFLAGS +=  -std=c++11

RESOURCES += mario.qrc


SOURCES += main.cpp \
    block.cpp \
    castle.cpp \
    flag.cpp \
    gameboard.cpp \
    wingedgoomba.cpp \
    floor.cpp \
    goomba.cpp \
    gameover.cpp \
    mario.cpp \
    monster.cpp \
    person.cpp \
    model.cpp \
    spike.cpp \
    splashscreen.cpp \
    background.cpp \
    won.cpp

HEADERS  += \
    block.h \
    castle.h \
    flag.h \
    floor.h \
    gameboard.h \
    wingedgoomba.h \
    goomba.h \
    gameover.h \
    mario.h \
    monster.h \
    person.h \
    model.h \
    spike.h \
    splashscreen.h \
    background.h \
    won.h

DISTFILES +=

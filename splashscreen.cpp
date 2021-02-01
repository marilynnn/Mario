#include "splashscreen.h"
#include <iostream>

SplashScreen::SplashScreen(int x, int y ) : Block(x,y, ":images/splashscreen.png")
{
    std::cout << ("SplashScreen created\n");
}

SplashScreen::~SplashScreen()
{
    std::cout << ("SplashScreen deleted\n");
}

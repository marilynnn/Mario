#include "question.h"
#include <iostream>

Question::Question(int x, int y ) : Block(x,y, ":images/question.png")
{}

Question::~Question()
{
    std::cout << ("Question deleted\n");
}

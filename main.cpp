#include <iostream>
#include "interpreter.h"

int main()
{
    Interpreter *interpreter = new Interpreter("input.txt");
    std::cout << interpreter->expr() << std::endl;
}

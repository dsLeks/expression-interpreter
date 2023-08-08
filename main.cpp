#include <iostream>
#include "interpreter.h"

int main()
{
    Interpreter *interpreter = new Interpreter("input.txt");
    interpreter->expr();
}

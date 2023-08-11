#include <iostream>
#include "interpreter.h"

int main()
{
    Interpreter *interpreter = new Interpreter("input.txt");
    interpreter->interpret();
}

// 7 + 3 * (10 / (12 / (3 + 1) - 1))
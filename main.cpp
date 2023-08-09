#include <iostream>
#include "interpreter.h"

int main()
{
    Interpreter *interpreter = new Interpreter("input.txt");
    ASTNode *root = interpreter->expr();
    std::cout << interpreter->printExpValue(root) << std::endl;
}

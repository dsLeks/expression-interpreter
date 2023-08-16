#include "interpreter.h"
#include <iostream>

Interpreter::Interpreter(const char *filename)
{
    parser = new Parser(filename, new Token(NONE, -1));
}

int Interpreter::traverse(ASTNode *root)
{
    if (root == nullptr)
        return INT_MIN;
    int leftVal = traverse(root->left);
    int rightVal = traverse(root->right);

    switch (root->node_type)
    {
    case INTEGER:
        return root->node_val;
    case PLUS:
        return leftVal + rightVal;
    case MINUS:
        return leftVal - rightVal;
    case MULTIPLY:
        return leftVal * rightVal;
    case DIVIDE:
        return leftVal / rightVal;
    default:
        return 0;
    }

    return INT_MIN;
}

void Interpreter::interpret()
{
    // ASTNode *root = parser->expr();                                               // parse the expression
    // int expression_value = traverse(root);                                        // calculate the expression value
    parser->parse();
}
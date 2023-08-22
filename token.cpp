#include "interpreter.h"

Token::Token(int type, int value)
{
    token_type = type;
    this->value = value;
};

std::string Token::token_to_string()
{

    if (token_type == INTEGER)
    {
        return "INTEGER: " + std::to_string(value);
    }

    if (token_type == PLUS)
    {
        return "PLUS";
    }

    if (token_type == MINUS)
    {
        return "MINUS";
    }

    if (token_type == MULTIPLY)
    {
        return "MULTIPLY";
    }

    if (token_type == DIVIDE)
    {
        return "DIVIDE";
    }

    if (token_type == LPAREN)
    {
        return "LPAREN";
    }

    if (token_type == RPAREN)
    {
        return "RPAREN";
    }

    if (token_type == BEGIN)
    {
        return "BEGIN";
    }

    if (token_type == DOT)
    {
        return "DOT";
    }

    if (token_type == END_OF_FILE)
        return "END_OF_FILE";

    return "NONE";
};
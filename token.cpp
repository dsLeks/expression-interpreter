#include "interpreter.h"

Token::Token(int type, int value)
{
    token_type = type;
    this->value = value;
};

std::string Token::token_to_string()
{
    char buffer[50];
    sprintf(buffer, "Token(%d, %d)", token_type, value);
    return std::string(buffer);
};
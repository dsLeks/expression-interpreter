#include "interpreter.h"
#include <iostream>

Lexer::Lexer(const char *filename)
{
    fp = fopen(filename, "r");
    pos = 0;
}

Token *Lexer::get_next_token()
{
    char ch;
    int value;

    ch = getc(fp);
    while (ch == ' ')
    {
        this->pos++;
        ch = getc(fp);
    }
    if (ch == EOF)
    {
        pos++;
        return new Token(END_OF_FILE, -1);
    }

    if (ch == '+')
    {
        pos++;
        return new Token(PLUS, (int)ch);
    }

    if (ch == '-')
    {
        pos++;
        return new Token(MINUS, (int)ch);
    }

    if (ch == '*')
    {
        pos++;
        return new Token(MULTIPLY, (int)ch);
    }

    if (ch == '(')
    {
        pos++;
        return new Token(LPAREN, (int)ch);
    }

    if (ch == ')')
    {
        pos++;
        return new Token(RPAREN, (int)ch);
    }

    if (ch == '/')
    {
        pos++;
        return new Token(DIVIDE, (int)ch);
    }

    while (ch >= '0' && ch <= '9')
    {
        pos++;
        value = ch - '0';
        while (true)
        {
            pos++;
            ch = fgetc(fp);
            if (ch >= '0' && ch <= '9')
            {
                value = value * 10 + (ch - '0');
            }
            else
            {
                pos--;
                ungetc(ch, fp);
                break;
            }
        }

        return new Token(INTEGER, value);
    }
}

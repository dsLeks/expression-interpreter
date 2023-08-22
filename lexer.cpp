#include "interpreter.h"
#include <iostream>

Lexer::Lexer(const char *filename)
{
    fp = fopen(filename, "r");
    pos = 0;
    reserved_keywords["BEGIN"] = 1;
    reserved_keywords["END"] = 1;
}

char Lexer::peek()
{
    char ch = getc(fp);
    ungetc(ch, fp);
    return ch;
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

    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
    {
        std::string str = "";
        while ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
        {
            str += ch;
            pos++;
            ch = fgetc(fp);
        }
        ungetc(ch, fp);
        if (reserved_keywords.find(str) != reserved_keywords.end())
        {
            if (str == "BEGIN")
            {
                return new Token(BEGIN, -1);
            }
            else if (str == "END")
            {
                return new Token(END, -1);
            }
        }
    }

    if (ch == '.')
        return new Token(DOT, -1);
    else if (ch == ':')
    {
        pos++;
        ch = fgetc(fp);
        if (ch == '=')
        {
            return new Token(ASSIGNMENT, -1);
        }
        else
        {
            ungetc(ch, fp);
            return new Token(NONE, -1);
        }
    }
    else if (ch == ';')
    {
        return new Token(SEMICOLON, -1);
    }
    else
    {
        return new Token(NONE, -1);
    }
}

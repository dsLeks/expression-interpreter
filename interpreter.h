#include <string>

enum
{
    INTEGER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LPAREN,
    RPAREN,
    END_OF_FILE,
    NONE
};

class Token
{
public:
    int token_type;
    int value;
    Token(){};
    Token(int type, int value)
    {
        this->token_type = type;
        this->value = value;
    };

    std::string token_to_string()
    {
        char buffer[50];
        sprintf(buffer, "Token(%d, %d)", this->token_type, this->value);
        return std::string(buffer);
    };
};

class Interpreter
{
public:
    FILE *fp;
    int pos;
    Token current_token;
    Interpreter(const char *filename)
    {
        this->fp = fopen(filename, "r");
        this->pos = 0;
        this->current_token = Token(NONE, -1);
    };

    void error()
    {
        printf("Error parsing input\n");
        exit(1);
    };

    Token get_next_token()
    {
        char ch;
        int value;

        ch = getc(this->fp);
        while (ch == ' ')
        {
            this->pos++;
            ch = getc(this->fp);
        }
        if (ch == EOF)
        {
            this->pos++;
            return Token(END_OF_FILE, -1);
        }

        if (ch == '+')
        {
            this->pos++;
            return Token(PLUS, (int)ch);
        }

        if (ch == '-')
        {
            this->pos++;
            return Token(MINUS, (int)ch);
        }

        if (ch == '*')
        {
            this->pos++;
            return Token(MULTIPLY, (int)ch);
        }

        if (ch == '(')
        {
            this->pos++;
            return Token(LPAREN, (int)ch);
        }

        if (ch == ')')
        {
            this->pos++;
            return Token(RPAREN, (int)ch);
        }

        if (ch == '/')
        {
            this->pos++;
            return Token(DIVIDE, (int)ch);
        }

        while (ch >= '0' && ch <= '9')
        {
            this->pos++;
            value = ch - '0';
            while (true)
            {
                this->pos++;
                ch = fgetc(this->fp);
                if (ch >= '0' && ch <= '9')
                {
                    value = value * 10 + (ch - '0');
                }
                else
                {
                    this->pos--;
                    ungetc(ch, this->fp);
                    break;
                }
            }
            return Token(INTEGER, value);
        }
    };
    void validate(int token_type)
    {
        if (this->current_token.token_type != token_type)
        {
            this->error();
        }
    }

    void factor()
    {
        this->current_token = get_next_token();
        int exprVal;
        if (current_token.token_type == LPAREN)
        {
            this->validate(LPAREN);
            exprVal = this->expr();
        }
        if (current_token.token_type == RPAREN)
        {
            this->validate(RPAREN);
            current_token.value = exprVal;
            current_token.token_type = INTEGER;
        }

        this->validate(INTEGER);
    }

    int term()
    {
        this->factor();
        int val = this->current_token.value;
        this->current_token = get_next_token();
        while (this->current_token.token_type == MULTIPLY || this->current_token.token_type == DIVIDE)
        {
            if (this->current_token.token_type == MULTIPLY)
            {
                this->validate(MULTIPLY);
                factor();
                val *= this->current_token.value;
            }
            else if (this->current_token.token_type == DIVIDE)
            {
                this->validate(DIVIDE);
                factor();
                val /= this->current_token.value;
            }
            this->current_token = this->get_next_token();
        }

        return val;
    }

    int expr()
    {
        int val = this->term();
        while (this->current_token.token_type == PLUS || this->current_token.token_type == MINUS)
        {
            if (this->current_token.token_type == PLUS)
            {
                this->validate(PLUS);
                val += this->term();
            }
            else if (this->current_token.token_type == MINUS)
            {
                this->validate(MINUS);
                val -= this->term();
            }
        }

        return val;
    };
};
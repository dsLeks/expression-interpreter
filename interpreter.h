#include <string>

enum
{
    INTEGER,
    PLUS,
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
    };

    Token get_next_token()
    {
        char ch;
        int value;

        ch = getc(this->fp);
        if (ch == EOF)
        {
            this->pos++;
            return Token(END_OF_FILE, -1);
        }
        if (ch == ' ')
        {
            this->pos++;
        }
        if (ch == '+')
        {
            this->pos++;
            return Token(PLUS, (int)ch);
        }
        if (ch >= '0' && ch <= '9')
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
    void eat(int token_type)
    {
        if (this->current_token.token_type == token_type)
        {
            this->current_token = this->get_next_token();
        };
    }

    int expr()
    {
        this->current_token = this->get_next_token();
        int left = this->current_token.value;
        this->eat(INTEGER);
        this->eat(PLUS);
        int right = this->current_token.value;

        return left + right;
    };
};
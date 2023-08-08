#include <string>

enum
{
    INTEGER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
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

    int expr()
    {
        this->current_token = Token(NONE, -1);
        int val = 0;
        int op = NONE;
        while (this->current_token.token_type != END_OF_FILE)
        {
            this->current_token = this->get_next_token();
            if (this->current_token.token_type == END_OF_FILE)
                break;
            switch (this->current_token.token_type)
            {
            case INTEGER:
                this->validate(INTEGER);
                if (op == NONE)
                    val = this->current_token.value;
                else
                {
                    switch (op)
                    {
                    case PLUS:
                        val += this->current_token.value;
                        break;
                    case MINUS:
                        val -= this->current_token.value;
                        break;
                    case MULTIPLY:
                        val *= this->current_token.value;
                        break;
                    case DIVIDE:
                        val /= this->current_token.value;
                        break;
                    };
                }
                break;
            case PLUS:
                this->validate(PLUS);
                op = PLUS;
                break;
            case MINUS:
                this->validate(MINUS);
                op = MINUS;
                break;
            case MULTIPLY:
                this->validate(MULTIPLY);
                op = MULTIPLY;
                break;
            case DIVIDE:
                this->validate(DIVIDE);
                op = DIVIDE;
                break;
            };
        }

        return val;
    };
};
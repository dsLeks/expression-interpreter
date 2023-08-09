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

class ASTNode
{
public:
    int node_type;
    int node_val;
    ASTNode *left;
    ASTNode *right;

    ASTNode(int type, int val)
    {
        this->node_type = type;
        this->node_val = val;
        this->left = nullptr;
        this->right = nullptr;
    }
    ASTNode(int type, int val, ASTNode *left, ASTNode *right)
    {
        this->node_type = type;
        this->node_val = val;
        this->left = left;
        this->right = right;
    };
};

class Interpreter
{
public:
    int expVal;
    FILE *fp;
    int pos;
    Token current_token;
    Interpreter(const char *filename)
    {
        this->fp = fopen(filename, "r");
        this->pos = 0;
        this->current_token = Token(NONE, -1);
        this->expVal = 0;
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
            // std::cout << "current_token.tokey_type: " << this->current_token.token_type << " token_type: " << token_type << "\n";
            this->error();
        }
    }

    // factor : INTEGER | LPAREN expr RPAREN
    ASTNode *factor()
    {
        this->current_token = get_next_token();

        //           << std::endl;
        // int exprVal;
        ASTNode *binaryNode = nullptr;
        if (current_token.token_type == LPAREN)
        {
            this->validate(LPAREN);
            binaryNode = this->expr();

            // this->current_token = get_next_token();
        }
        if (current_token.token_type == RPAREN)
        {

            this->validate(RPAREN);

            return binaryNode;
        }

        this->validate(INTEGER);
        // std::cout << "failing here!! " << std::endl
        //           << std::endl;
        return new ASTNode(INTEGER, this->current_token.value);
    }

    // term : factor ((MULTIPLY | DIVIDE) factor)*
    ASTNode *term()
    {
        ASTNode *left = this->factor();
        ASTNode *binaryNode = nullptr;
        // int val = this->current_token.value;
        this->current_token = get_next_token();
        while (this->current_token.token_type == MULTIPLY || this->current_token.token_type == DIVIDE)
        {
            if (this->current_token.token_type == MULTIPLY)
            {
                this->validate(MULTIPLY);
                ASTNode *right = this->factor();
                binaryNode = new ASTNode(MULTIPLY, '*', left, right);
            }
            else if (this->current_token.token_type == DIVIDE)
            {
                // std::cout << "HERE!" << std::endl;
                this->validate(DIVIDE);

                ASTNode *right = this->factor();
                binaryNode = new ASTNode(DIVIDE, '/', left, right);
            }

            left = binaryNode;
            this->current_token = get_next_token();
        }

        return left;
    }

    int printExpValue(ASTNode *root)
    {
        if (root == nullptr)
            return INT_MIN;
        int leftVal = printExpValue(root->left);
        int rightVal = printExpValue(root->right);

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

    // expr : term ((PLUS | MINUS) term)*
    ASTNode *expr()
    {
        ASTNode *left = this->term();
        ASTNode *binaryNode = nullptr;
        while (this->current_token.token_type == PLUS || this->current_token.token_type == MINUS)
        {
            if (this->current_token.token_type == PLUS)
            {
                this->validate(PLUS);
                ASTNode *right = this->term();
                binaryNode = new ASTNode(PLUS, '+', left, right);
            }
            else if (this->current_token.token_type == MINUS)
            {
                this->validate(MINUS);
                ASTNode *right = this->term();
                binaryNode = new ASTNode(MINUS, '-', left, right);
            }

            left = binaryNode;
            // this->current_token = get_next_token();
        }

        return left;
    };
};
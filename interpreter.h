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

    // factor : INTEGER | LPAREN expr RPAREN
    ASTNode *factor()
    {
        this->current_token = get_next_token();
        int exprVal;
        if (current_token.token_type == LPAREN)
        {
            this->validate(LPAREN);
            this->expr();
        }
        if (current_token.token_type == RPAREN)
        {
            this->validate(RPAREN);
            current_token.value = exprVal;
            current_token.token_type = INTEGER;
        }

        this->validate(INTEGER);
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
                this->validate(DIVIDE);
                ASTNode *right = this->factor();
                binaryNode = new ASTNode(DIVIDE, '*', left, right);
            }
            this->current_token = this->get_next_token();
        }

        return (binaryNode == nullptr) ? left : binaryNode;
    }

    void printTree(ASTNode *root)
    {
        if (root == nullptr)
            return;
        printTree(root->left);
        std::cout << "Node Type is: " << root->node_type << " Node Value is: " << root->node_val << std::endl;
        printTree(root->right);
    }

    // expr : term ((PLUS | MINUS) term)*
    void expr()
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
        }

        printTree(left);
    };
};
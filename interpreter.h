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
    BEGIN,
    END,
    DOT,
    ASSIGNMENT,
    SEMICOLON,
    IDENTIFIER,
    END_OF_FILE,
    NONE
};

class Token
{
public:
    int token_type;
    int value;
    Token(){};
    Token(int type, int value);
    std::string token_to_string();
};

class Lexer
{
public:
    int pos; // index into text
    FILE *fp;
    Lexer(const char *filename);
    void peek();
    Token *get_next_token();
    void validate();
};

class ASTNode
{
public:
    int node_type;
    int node_val;
    ASTNode *left;
    ASTNode *right;
    ASTNode(int type, int val);
    ASTNode(int type, int val, ASTNode *left, ASTNode *right);
};

class Parser
{
public:
    Lexer *lexer;
    Token *current_token;
    Parser(const char *filename, Token *start_token);
    void error();
    void validate(int token_type);
    ASTNode *factor();
    ASTNode *term();
    ASTNode *expr();
};

class Interpreter
{
public:
    Lexer *lexer;
    Parser *parser;
    Interpreter(const char *filename);
    int traverse(ASTNode *root); // For now it traverses the tree and returns the value of an expression
    void interpret();
};
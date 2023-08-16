#include "interpreter.h"
#include <iostream>

Parser::Parser(const char *filename, Token *start_token)
{
    lexer = new Lexer(filename);
    current_token = start_token;
};

void Parser::error()
{
    printf("Error parsing input\n");
    exit(1);
};

void Parser::validate(int token_type)
{
    if (current_token->token_type != token_type)
    {
        error();
    }
};

// factor : (PLUS | MINUS) factor | INTEGER | LPAREN expr RPAREN
ASTNode *Parser::factor()
{
    current_token = lexer->get_next_token();
    ASTNode *binaryNode = nullptr;
    if (current_token->token_type == PLUS)
    {
        validate(PLUS);
        ASTNode *node = new ASTNode(PLUS, current_token->value);
        node->right = factor();
        node->left = new ASTNode(INTEGER, 0);
        return node;
    }
    if (current_token->token_type == MINUS)
    {

        validate(MINUS);
        ASTNode *node = new ASTNode(MINUS, current_token->value);
        node->right = factor();
        node->left = new ASTNode(INTEGER, 0);
        return node;
    }

    if (current_token->token_type == LPAREN)
    {
        validate(LPAREN);
        binaryNode = expr();

        // this->current_token = get_next_token();
    }
    if (current_token->token_type == RPAREN)
    {

        validate(RPAREN);

        return binaryNode;
    }

    validate(INTEGER);

    return new ASTNode(INTEGER, current_token->value);
}

// term : factor ((MULTIPLY | DIVIDE) factor)*
ASTNode *Parser::term()
{
    ASTNode *left = factor();
    ASTNode *binaryNode = nullptr;
    // int val = this->current_token->value;
    current_token = lexer->get_next_token();
    while (current_token->token_type == MULTIPLY || current_token->token_type == DIVIDE)
    {
        if (current_token->token_type == MULTIPLY)
        {
            validate(MULTIPLY);
            ASTNode *right = factor();
            binaryNode = new ASTNode(MULTIPLY, '*', left, right);
        }
        else if (current_token->token_type == DIVIDE)
        {

            validate(DIVIDE);

            ASTNode *right = factor();
            binaryNode = new ASTNode(DIVIDE, '/', left, right);
        }

        left = binaryNode;
        current_token = lexer->get_next_token();
    }

    return left;
}

// expr : term ((PLUS | MINUS) term)*
ASTNode *Parser::expr()
{
    ASTNode *left = term();
    ASTNode *binaryNode = nullptr;
    while (current_token->token_type == PLUS || current_token->token_type == MINUS)
    {
        if (current_token->token_type == PLUS)
        {
            validate(PLUS);
            ASTNode *right = term();
            binaryNode = new ASTNode(PLUS, '+', left, right);
        }
        else if (current_token->token_type == MINUS)
        {
            validate(MINUS);
            ASTNode *right = term();
            binaryNode = new ASTNode(MINUS, '-', left, right);
        }

        left = binaryNode;
    }

    return left;
}

ASTNode *Parser::parse()
{
    current_token = lexer->get_next_token();
    if (current_token->token_type == BEGIN)
    {
        std::cout << "BEGIN" << std::endl;
    }
}
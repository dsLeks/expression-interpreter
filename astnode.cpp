#include "interpreter.h"

ASTNode::ASTNode(int type, int val)
{
    this->node_type = type;
    this->node_val = val;
    this->left = nullptr;
    this->right = nullptr;
}
ASTNode::ASTNode(int type, int val, ASTNode *left, ASTNode *right)
{
    this->node_type = type;
    this->node_val = val;
    this->left = left;
    this->right = right;
};
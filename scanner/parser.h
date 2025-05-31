#ifndef PARSER_H
#define PARSER_H

#include "tokens.h"

typedef enum {
    NODE_PROGRAM,
    NODE_STATEMENT_LIST,
    NODE_EXPRESSION,
    NODE_ASSIGNMENT,
    NODE_IF,
    NODE_WHILE,
    NODE_FOR,
    NODE_BINARY_OP,
    NODE_UNARY_OP,
    NODE_INT_LITERAL,
    NODE_VARIABLE,
    NODE_COMPOUND_STMT,
    NODE_RETURN,
    NODE_TYPE,
    NODE_DOUBLE_LITERAL,
    NODE_STRING_LITERAL,
} NodeType;

typedef struct ASTNode {
    NodeType type;
    Token token;
    struct ASTNode* left;
    struct ASTNode* right;
    struct ASTNode* extra; // For ternary ops or additional nodes
    struct ASTNode* next;  // For statement lists
} ASTNode;

// Parser functions
ASTNode* parse_program();
void free_ast(ASTNode* node);
void print_ast(ASTNode* node, int indent);
void print_ast_pointers(ASTNode* node, int indent); 

// Error handling
void parser_error(const char* message);
int parser_had_error();

#endif
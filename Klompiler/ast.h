// ast.h
#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack_of_tries.h"


typedef enum {
    NODE_PROGRAM,
    NODE_DECLARATION,
    NODE_ASSIGNMENT,
    NODE_IDENTIFIER,
    NODE_INTEGER,
    NODE_FLOAT,
    NODE_STRING,
    NODE_CHAR,
    NODE_BINARY_OP,
    NODE_UNARY_OP,
    NODE_IF,
    NODE_WHILE,
    NODE_FOR,
    NODE_BREAK,
    NODE_RETURN,
    NODE_COMPOUND,
    NODE_FUNCTION,
    NODE_CALL, 
    NODE_CONTINUE
} NodeType;

typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_ASSIGN,
    OP_EQ,
    OP_NE,
    OP_LT,
    OP_LE,
    OP_GT,
    OP_GE,
    OP_AND,
    OP_OR,
    OP_NOT,
    OP_NEG, OP_PRE_INC, OP_PRE_DEC, OP_POST_INC, OP_POST_DEC,
    OP_INC,
    OP_DEC,
    OP_PE,   // +=
    OP_ME,   // -=
    OP_MULE, // *=
    OP_DIVE, // /=
    OP_MODE,  // %=
} OperatorType;

typedef enum {
    TYPE_INT,
    TYPE_LONG,
    TYPE_LONGLONG,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_LONGDOUBLE,
    TYPE_CHAR,
    TYPE_STR,
} VarType;

typedef struct ASTNode {
    NodeType type;
    int line_number;
    ScopeStack *symbol_table;
    
    union {
        // For literals and identifiers
        struct {
            char *string_value;
            long long int_value;
            double float_value;
            char char_value;
        };
        
        // For binary/unary operations
        struct {
            OperatorType op;
            struct ASTNode *left;
            struct ASTNode *right;
        };
        
        // For control structures
        struct {
            struct ASTNode *cond;
            struct ASTNode *then_part;
            struct ASTNode *else_part;
            struct ASTNode *init;
            struct ASTNode *step;
        };
        
        // For declarations
        struct {
            char *var_name;
            int var_type;  // You can define an enum for types
            bool is_constant;
            struct ASTNode *init_value;
        };
        
        // For compound statements
        struct {
            struct ASTNode **children;
            size_t children_count;
        };
    };
} ASTNode;

// AST creation functions
ASTNode *ast_new_program(ScopeStack *symbol_table);
ASTNode *ast_new_declaration(char *name, int type, ASTNode *init_value, int line);
ASTNode *ast_new_identifier(char *name, int line);
ASTNode *ast_new_integer(long long value, int line);
ASTNode *ast_new_float(double value, int line);
ASTNode *ast_new_string(char *value, int line);
ASTNode *ast_new_char(char value, int line);
ASTNode *ast_new_binary_op(OperatorType op, ASTNode *left, ASTNode *right, int line);
ASTNode *ast_new_unary_op(OperatorType op, ASTNode *expr, int line);
ASTNode *ast_new_if(ASTNode *cond, ASTNode *then_part, ASTNode *else_part, int line);
ASTNode *ast_new_while(ASTNode *cond, ASTNode *body, int line);
ASTNode *ast_new_for(ASTNode *init, ASTNode *cond, ASTNode *step, ASTNode *body, int line);
ASTNode *ast_new_compound_statement(int line);
ASTNode *ast_add_statement(ASTNode *compound, ASTNode *stmt);
ASTNode *ast_new_continue(int line);
ASTNode *ast_new_break(int line);
ASTNode *ast_new_return(ASTNode* expr, int line);
void ast_free(ASTNode *node);

// Semantic analysis
void semantic_analyze(ASTNode *node, ScopeStack *symbol_table);
void ast_print(ASTNode *node, int indent);

// Code generation
void generate_assembly(ASTNode *node, FILE *out);

#endif
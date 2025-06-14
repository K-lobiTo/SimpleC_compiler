#ifndef COMPILER_H
#define COMPILER_H

#include <stdlib.h>
#include <string.h>

struct ast_node;
typedef struct ast_node ASTNode;

typedef enum {
    NODE_INT,
    NODE_IDENT,
    NODE_BINOP,
    NODE_ASSIGN,
    NODE_IF,
    NODE_WHILE,
    NODE_FOR,
    NODE_BREAK,
    NODE_RETURN,
    NODE_COMPOUND,
    NODE_DECL,
    NODE_DECL_INIT,
    NODE_FUNCTION,
    NODE_PREFIX_OP,
    NODE_POSTFIX_OP,
    NODE_COMPOUND_ASSIGN,
    NODE_UNARY_OP
} NodeType;

typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_EQ,
    OP_NE,
    OP_LT,
    OP_LE,
    OP_GT,
    OP_GE,
    OP_AND,
    OP_OR,
    OP_NOT,
    OP_INC,
    OP_DEC,
    OP_PE,
    OP_ME,
    OP_MULE,
    OP_DIVE,
    OP_MODE,
} Operator;


/* Error token */
#define ERROR 256

struct ast_node {
    NodeType type;
    union {
        /* For integers and identifiers */
        struct {
            int value;
            char *name;
        };
        
        /* For binary operations */
        struct {
            ASTNode *left;
            ASTNode *right;
            Operator op;
        };
        
        /* For assignment */
        struct {
            ASTNode *lvalue;
            ASTNode *rvalue;
        };
        
        /* For control structures */
        struct {
            ASTNode *cond;
            ASTNode *then_part;
            ASTNode *else_part;
            ASTNode *init;
            ASTNode *step;
        };
        
        /* For compound statements */
        struct {
            ASTNode **stmts;
            int stmt_count;
        };
        
        /* For function definitions */
        struct {
            char *func_name;
            ASTNode *func_params;
            ASTNode *func_body;
        };

        // For prefix/postfix ops and compound assignments
        struct {
            Operator unary_op;
            ASTNode *unary_expr; // this might do everything fail at some point (hopefully no)
            char * unary_name;
        };

        /* For compound assignments */
        struct {
            char *compound_name;
            Operator compound_op;
            ASTNode *compound_rvalue;
        };
    };
};


// I need to add YYLTYPE loc to all of this later
ASTNode *new_int_node(int value);
ASTNode *new_ident_node(char *name);
ASTNode *new_not_node(ASTNode *expr);
ASTNode *new_binop_node(Operator op, ASTNode *left, ASTNode *right);
ASTNode *new_assign_node(ASTNode *lvalue, ASTNode *rvalue);
ASTNode *new_if_node(ASTNode *cond, ASTNode *then_part, ASTNode *else_part);
ASTNode *new_while_node(ASTNode *cond, ASTNode *body);
ASTNode *new_for_node(ASTNode *init, ASTNode *cond, ASTNode *step, ASTNode *body);
ASTNode *new_break_node();
ASTNode *new_return_node(ASTNode *expr);
ASTNode *new_compound_node(ASTNode **stmts, int stmt_count);
ASTNode *new_decl_node(char *name);
ASTNode *new_decl_init_node(char *name, ASTNode *init);
ASTNode *new_prefix_op_node(Operator op, char *ident);
ASTNode *new_postfix_op_node(Operator op, char *ident);
ASTNode *new_compound_assign_node(char *ident, Operator op, ASTNode *expr);
ASTNode *new_main_function_node(char *name, ASTNode *params, ASTNode *body);

void free_ast(ASTNode *node);

// Error handling

void compiler_error(const char *msg);
void print_errors(void);
char process_char(const char *text);
#endif
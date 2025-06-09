#include "compiler.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

ASTNode *new_int_node(int value) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_INT;
    node->value = value;
    return node;
}

ASTNode *new_ident_node(char *name) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_IDENT;
    node->name = strdup(name);
    return node;
}

ASTNode *new_binop_node(Operator op, ASTNode *left, ASTNode *right) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_BINOP;
    node->op = op;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode *new_assign_node(ASTNode *lvalue, ASTNode *rvalue) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_ASSIGN;
    node->lvalue = lvalue;
    node->rvalue = rvalue;
    return node;
}

ASTNode *new_if_node(ASTNode *cond, ASTNode *then_part, ASTNode *else_part) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_IF;
    node->cond = cond;
    node->then_part = then_part;
    node->else_part = else_part;
    return node;
}

ASTNode *new_while_node(ASTNode *cond, ASTNode *body) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_WHILE;
    node->cond = cond;
    node->then_part = body; // Reusing then_part as body
    return node;
}

ASTNode *new_for_node(ASTNode *init, ASTNode *cond, ASTNode *step, ASTNode *body) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_FOR;
    node->init = init;
    node->cond = cond;
    node->step = step;
    node->then_part = body; // Reusing then_part as body
    return node;
}

ASTNode *new_break_node() {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_BREAK;
    return node;
}

ASTNode *new_not_node(ASTNode *expr) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_UNARY_OP;
    node->unary_op = OP_NOT;
    node->unary_expr = expr;     // Add this to your union if not present
    return node;
}

ASTNode *new_return_node(ASTNode *expr) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_RETURN;
    node->then_part = expr; // Reusing then_part as expr
    return node;
}

ASTNode *new_compound_node(ASTNode **stmts, int stmt_count) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_COMPOUND;
    node->stmts = stmts;
    node->stmt_count = stmt_count;
    return node;
}

ASTNode *new_decl_node(char *name) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_DECL;
    node->name = strdup(name);
    return node;
}

ASTNode *new_decl_init_node(char *name, ASTNode *init) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_DECL_INIT;
    node->name = strdup(name);
    node->then_part = init; // Reusing then_part as init
    return node;
}

ASTNode *new_prefix_op_node(Operator op, char *ident) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_PREFIX_OP;
    node->unary_op = op;
    node->unary_name = strdup(ident);
    return node;
}

ASTNode *new_postfix_op_node(Operator op, char *ident) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_POSTFIX_OP;
    node->unary_op = op;
    node->unary_name = strdup(ident);
    return node;
}

ASTNode *new_compound_assign_node(char *ident, Operator op, ASTNode *expr) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_COMPOUND_ASSIGN;
    node->compound_op = op;
    node->compound_name = strdup(ident);
    node->compound_rvalue = expr;
    return node;
}

ASTNode *new_main_function_node(char *name, ASTNode *params, ASTNode *body) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_FUNCTION;
    node->func_name = strdup(name);
    node->func_params = params;
    node->func_body = body;
    return node;
}

void free_ast(ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_IDENT:
        case NODE_DECL:
        case NODE_DECL_INIT:
            free(node->name);
            break;
            
        case NODE_BINOP:
            free_ast(node->left);
            free_ast(node->right);
            break;
            
        case NODE_ASSIGN:
            free_ast(node->lvalue);
            free_ast(node->rvalue);
            break;
            
        case NODE_IF:
            free_ast(node->cond);
            free_ast(node->then_part);
            free_ast(node->else_part);
            break;
            
        case NODE_WHILE:
        case NODE_FOR:
        case NODE_RETURN:
            free_ast(node->cond);
            free_ast(node->then_part);
            free_ast(node->init);
            free_ast(node->step);
            break;
            
        case NODE_COMPOUND:
            for (int i = 0; i < node->stmt_count; i++) {
                free_ast(node->stmts[i]);
            }
            free(node->stmts);
            break;
        case NODE_PREFIX_OP:
        case NODE_POSTFIX_OP:
            free(node->unary_name);
            break;
            
        case NODE_COMPOUND_ASSIGN:
            free(node->compound_name);
            free_ast(node->compound_rvalue);
            break;
        case NODE_UNARY_OP:
            if (node->unary_expr) {
                free_ast(node->unary_expr);
            }
            if (node->unary_name) {
                free(node->unary_name);
            }
            break;    
            
        default:
            break;
    }
    
    free(node);
}

// Error handling
static char *last_error = NULL;

void compiler_error(const char *msg) {
    if (last_error)print_errors();
    last_error = strdup(msg);
    print_errors();
}

void print_errors(void) {
    if (last_error) {
        fprintf(stderr, "Error: %s\n", last_error);
        free(last_error);
        last_error = NULL;
    }
}
